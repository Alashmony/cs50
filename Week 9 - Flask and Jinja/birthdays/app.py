import os
import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    errorm = ""
    friends = db.execute("SELECT * FROM birthdays")
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Check if there are values or not
        if not name or not month or not day:
            errorm = "You must complete all fields"
            return render_template("index.html", friends=friends, errorm=errorm)

        try:
            datetime.date(2020, int(month), int(day))
            errorm = ""
            db.execute("INSERT INTO birthdays (name, month, day) values (?,?,?)", name, month, day)
            return render_template("index.html", friends=friends, errorm=errorm)

        except:
            errorm = "You must insert a valid date"
            return render_template("index.html", friends=friends, errorm=errorm)

        # return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        errorm = ""
        return render_template("index.html", friends=friends, errorm=errorm)
