import os
import regex
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    # Get username and cash
    userid = session["user_id"]
    user = db.execute("SELECT username, round(cash, 2) cash FROM users WHERE id = ?", userid)
    username = user[0]["username"]
    cash = user[0]["cash"]

    # Get stocks in hand and their amounts
    stocks = db.execute(
        "SELECT stock, ROUND(SUM(amount), 2) amount, round(sum(value), 2) buy_price from transactions where user = ? group by stock;", userid)
    i = 0
    total_stocks = 0
    for stock in stocks:
        symbol = stock["stock"]
        stock_price = lookup(symbol)
        stocks[i]["price"] = usd(stock_price["price"])
        stock_total = stock_price["price"] * stock["amount"]
        total_stocks += stock_total
        stocks[i]["total"] = usd(stock_total)
        # stocks[i]["prof_loss"] = stocks[i]["total"] - stock["buy_price"]
        i += 1

    notification = request.args.get("notification")
    wealth = total_stocks + float(cash)
    if not notification:
        return render_template("index.html", username=username, cash=cash, stocks=stocks)

    return render_template("index.html", username=username, cash=usd(cash), stocks=stocks, notification=notification, total_stocks=usd(total_stocks), portfolio=usd(wealth))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        amount = request.form.get("shares")

        try:
            amount = int(amount)
        except:
            return apology("You must enter a valid amount!")

        if int(amount) < 1:
            return apology("Please enter a valid amount")

        stock = lookup(symbol)

        if not stock:
            return apology("Please enter a valid stock symbol")

        user = session["user_id"]
        stock_price = float(stock["price"])
        stocks_val = int(amount) * stock_price
        timestamp = datetime.datetime.now()

        user_cash = db.execute("SELECT cash FROM users where id = ?;", user)
        cash = float(user_cash[0]["cash"])
        print(cash)

        if float(cash) < float(stocks_val):
            return apology("Sorry, You do not have enough cash!")

        remaining_amount = cash - stocks_val
        db.execute("UPDATE users SET cash = ? where id = ?;", remaining_amount, user)
        db.execute("INSERT INTO transactions(user, stock, amount, price, value, buy, timestamp) values (?,?,?,?,?,?,?)",
                   user, symbol, amount, stock_price, stocks_val, True, timestamp)

        return redirect(url_for("index", notification="Bought!"))
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    # Make clear whether a stock was bought or sold and include the stock’s symbol,
    # the (purchase or sale) price, the number of shares bought or sold, and the date and time at which the transaction occurred -->
    """Show history of transactions"""
    user = session["user_id"]
    user_data = db.execute("SELECT username, cash FROM users WHERE id = ?", user)
    username = user_data[0]["username"]
    cash = float(user_data[0]["cash"])
    transactions = db.execute(
        "SELECT (CASE WHEN buy = true then 'Buy' ELSE 'SELL' END) as type , stock, abs(price) price, abs(amount) amount, timestamp from transactions WHERE user = ?;", user)
    return render_template("history.html", username=username, cash=usd(cash), transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("You must enter a stock symbol")

        stock = lookup(symbol)
        if not stock:
            return apology("Enter a valid stock symbol!")
        stock["price"] = usd(stock["price"])

        return render_template("quote.html", stock=stock, quoted=False)

    return render_template("quote.html", quoted=True)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        user = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not user or not password or not confirmation:
            return apology("All fields must be complete")

        if password != confirmation:
            return apology("Password fields does not match")

        if len(password) < 6:
            return apology("Password must be at least 6 characters")

        # Copied from online resources and modified to match my requirements :D
        # pass_pattern = r"^(?=.*\d)(?=.*[A-Z])(?=.*[a-z])(?=.*[@#$%^&+=])[A-Za-z0-9@#$%^&+=]{6,}$"
        # if not regex.fullmatch(pass_pattern, password):
        #    return apology("Password must be at least 6 characters including at least a capital letter, a small letter, an integer, and a special character.")

        user_exists = db.execute("SELECT username FROM users WHERE username = ?;", user)
        if len(user_exists) > 0:
            return apology("This username is already taken, try another one!")
        pass_hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) values (?, ?)", user, pass_hash)

        return render_template("register.html", registered="User created successfully, return to ", log="Log In")

    """Register user"""
    return render_template("register.html", registered="")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user = session["user_id"]
    if request.method == "GET":
        stocks = db.execute(
            "SELECT stock, sum(amount) amount FROM transactions WHERE user = ? GROUP BY stock HAVING sum(amount) > 0;", user)
        return render_template("sell.html", stocks=stocks)

    symbol = request.form.get("symbol")

    amount = int(request.form.get("shares"))
    if amount < 0:
        return apology("You entered an invalid amount")

    amount_in_hand = db.execute(
        "SELECT sum(amount) amount FROM transactions WHERE stock = ? and user = ?", symbol, user)[0]["amount"]
    if (not amount_in_hand) or int(amount_in_hand) < amount:
        return apology("You do not have enough stocks to sell")

    stock_price = lookup(symbol)["price"]
    if not stock_price:
        return apology("Cannot find this stock, check the symbol and try again!")

    value = float(stock_price) * amount
    timestamp = datetime.datetime.now()

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]["cash"]
    cash_after = float(cash) + value

    db.execute("INSERT INTO transactions (user, stock, amount, price, value, buy, timestamp) values(?,?,?,?,?,?,?)",
               user, symbol, amount*-1, float(stock_price)*-1, value*-1, False, timestamp)
    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after, user)

    return redirect(url_for("index", notification="Sold!"))
