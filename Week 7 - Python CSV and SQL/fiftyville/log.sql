-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Starting as Carter descriped, with the description
.tables
/*
airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers
*/

.schema crime_scene_reports

select * from crime_scene_reports;

select description from crime_scene_reports where street = 'Humphrey Street' and year = 2023 and month = 7 and day = 28;


/*
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
    Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.
*/
--Backery is a keyword here
-- Time is 10:15 AM

-- Let's execlude who has calls at that time
--Check table schema in another way
select * from phone_calls LIMIT 1;

-- Time is not in the calls, lets get the calls at that date anyway
select * from phone_calls WHERE year = 2023 and month = 7 and day = 28;

-- A lot here... let's see the Airport first
SELECT * FROM flights LIMIT 1;

-- We should see the airport names also
SELECT * FROM airports LIMIT 1;

-- Extremely intersting, lets see Airports from - to with names
SELECT o.full_name origin_name, o.city origin_city, d.full_name destination_name, d.city destination_city, f.year, f.month, f.day, f.hour, f.minute
FROM flights f
JOIN airports o ON f.origin_airport_id = o.id
JOIN airports d ON f.destination_airport_id = d.id;

/*
airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers
*/

-- Let's see people here, then we can check their financials and calls
SELECT * FROM people LIMIT 1;

-- We can join them with the phone #, passport # and license plate -- Start with passport #
SELECT * FROM passengers LIMIT 1;

-- Lets check ig the join works fine with flights
with flis as
(
    SELECT f.id, o.full_name origin_name, o.city origin_city, d.full_name destination_name, d.city destination_city, f.year, f.month, f.day, f.hour, f.minute
    FROM flights f
    JOIN airports o ON f.origin_airport_id = o.id
    JOIN airports d ON f.destination_airport_id = d.id
)
SELECT *
FROM passengers p
JOIN flis f ON p.flight_id = f.id;

-- Intersting, lets get the person name and choose the needed columns
with flis as
(
    SELECT f.id, o.full_name origin_name, o.city origin_city, d.full_name destination_name, d.city destination_city, f.year, f.month, f.day, f.hour, f.minute
    FROM flights f
    JOIN airports o ON f.origin_airport_id = o.id
    JOIN airports d ON f.destination_airport_id = d.id
)
SELECT pp.name, p.seat, origin_name, origin_city, destination_name, destination_city, f.year, f.month, f.day, f.hour, f.minute
FROM passengers p
JOIN flis f ON p.flight_id = f.id
JOIN people pp ON pp.passport_number = p.passport_number;


-- Lets execlude who got out before the crime and who entered after the crime
with flis as
(
    SELECT f.id, o.full_name origin_name, o.city origin_city, d.full_name destination_name, d.city destination_city, f.year, f.month, f.day, f.hour, f.minute
    FROM flights f
    JOIN airports o ON f.origin_airport_id = o.id
    JOIN airports d ON f.destination_airport_id = d.id
),
log as (
SELECT pp.name, p.seat, origin_name, origin_city, destination_name, destination_city, f.year, f.month, f.day, f.hour, f.minute
FROM passengers p
JOIN flis f ON p.flight_id = f.id
JOIN people pp ON pp.passport_number = p.passport_number)
SELECT *
FROM log
WHERE
--entered before crime
(destination_city = 'Fiftyville' and (day < 28 or (day = 28 and hour < 10 )))
or
--Left after crime
(origin_city = 'Fiftyville' and (day > 28 or (day = 28 and hour >= 10 )));


-- Too much
-- Lets check the bank
SELECT * FROM bank_accounts LIMIT 1;

-- When the bank accounts was made?
select distinct creation_year from bank_accounts;

-- Check the backery security logs can be a good point
select * from bakery_security_logs limit 1;

-- It seems that we can find people entered there by plate #
SELECT activity, name, hour, minute
FROM bakery_security_logs bsl
JOIN people ON bsl.license_plate = people.license_plate
WHERE year = 2023 AND month  = 7 AND day = 28 AND (hour < 10 or (hour = 10 and minute < 15)) and activity = 'entrance';

-- Who got out before the crime?
SELECT activity, name, hour, minute
FROM bakery_security_logs bsl
JOIN people ON bsl.license_plate = people.license_plate
WHERE year = 2023 AND month  = 7 AND day = 28 AND (hour < 10 or (hour = 10 and minute < 15)) and activity = 'exit';

-- WOW, we can suspect a very few with this
WITH exited as
(
    SELECT activity, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people ON bsl.license_plate = people.license_plate
    WHERE year = 2023 AND month  = 7 AND day = 28 AND (hour < 10 or (hour = 10 and minute < 15)) and activity = 'exit'
),
entered as
(
    SELECT activity, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people ON bsl.license_plate = people.license_plate
    WHERE year = 2023 AND month  = 7 AND day = 28 AND (hour < 10 or (hour = 10 and minute < 15)) and activity = 'entrance'
)
SELECT *
FROM entered
WHERE entered.name NOT IN (SELECT name FROM exited);

/*
+----------+---------+------+--------+
| activity |  name   | hour | minute |
+----------+---------+------+--------+
| entrance | Iman    | 8    | 18     |
| entrance | Bruce   | 8    | 23     |
| entrance | Taylor  | 8    | 34     |
| entrance | Diana   | 8    | 36     |
| entrance | Kelsey  | 8    | 42     |
| entrance | Luca    | 9    | 14     |
| entrance | Vanessa | 9    | 15     |
| entrance | Barry   | 9    | 20     |
| entrance | Sofia   | 9    | 28     |
| entrance | Brandon | 10   | 8      |
| entrance | Sophia  | 10   | 14     |
+----------+---------+------+--------+
*/

-- Lets see what we have in the interviews also
SELECT * FROM interviews LIMIT 1;

SELECT name, transcript
FROM interviews
WHERE year >= 2023 AND month >= 7 AND day >= 28 and lower(transcript) like '%bakery%';

/*
 Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
        If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |

I don't know the thief's name, but it was someone I recognized.
    Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |

As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
    In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
    The thief then asked the person on the other end of the phone to purchase the flight ticket.*/

-- Wow A lot of info again, let's get each of those info, and using CTEs, we can join them to get to the thief by keeping a peron ID (passport # /plate...etc) in the query

--1.  Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
--        If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Who left between 10.15 and 10.26

-- The previous query showed them without the plat number, let's check what was this column's name
select * from bakery_security_logs bsl limit 1;

--Let's use the previous query with license_plate in it
SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
FROM bakery_security_logs bsl
JOIN people p ON bsl.license_plate = p.license_plate
WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
    AND minute BETWEEN 15 AND 25;

-- We have only 8 here, let us see what else do we have

-- 2. I don't know the thief's name, but it was someone I recognized.
--    Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- We did not invistigate the financial tables before except the bank accounts, lets see what we have there
.tables
/*
airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers
*/

SELECT * FROM atm_transactions LIMIT 1;

-- Lets check the ATM locations
SELECT DISTINCT atm_location FROM atm_transactions;

--  Leggett Street is called as described within the interview transcript, and the transaction was a withdrawal

SELECT *
FROM atm_transactions
WHERE atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
    AND year = 2023
    AND month = 7
    AND day = 28;

-- We have only 8 also, seems intersting, lets get those people's identifiers
SELECT * FROM bank_accounts LIMIT 1;

SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
FROM atm_transactions t
JOIN bank_accounts a ON t.account_number = a.account_number
JOIN people p ON p.id = a.person_id
WHERE atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
    AND year = 2023
    AND month = 7
    AND day = 28;

SELECT * FROM bank_accounts LIMIT 1;

-- It seems good, let us check if we need the third one
with exiters as
(
    SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people p ON bsl.license_plate = p.license_plate
    WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
        AND minute BETWEEN 15 AND 25
),
withdrawers as
(
    SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
    FROM atm_transactions t
    JOIN bank_accounts a ON t.account_number = a.account_number
    JOIN people p ON p.id = a.person_id
    WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND year = 2023
        AND month = 7
        AND day = 28
)
SELECT e.person_id, e.name
FROM exiters e
INNER JOIN withdrawers w ON e.person_id = w.person_id;


-- We have 4 suspected people, we need the third piece of info
-- 3. As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
--    In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--    The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Let's check the call from the calls first
SELECT * FROM phone_calls LIMIT 1;

-- Now, we know that the call was outgoing, not inboud (he/she called someone as per the transcript)
SELECT *
FROM phone_calls
WHERE year = 2023
    AND month = 7
    AND day = 28;

-- Those are too much, let us check if someone from those suspected people has calls and his number is the caller, the reciever is the ACCOMPLICE
with exiters as
(
    SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people p ON bsl.license_plate = p.license_plate
    WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
        AND minute BETWEEN 15 AND 25
),
withdrawers as
(
    SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
    FROM atm_transactions t
    JOIN bank_accounts a ON t.account_number = a.account_number
    JOIN people p ON p.id = a.person_id
    WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND year = 2023
        AND month = 7
        AND day = 28
),
callers as
(SELECT *
FROM phone_calls
WHERE year = 2023
    AND month = 7
    AND day = 28
)
SELECT e.person_id, e.name, c.caller, c.receiver, c.duration, c.year, c.month, c.day
FROM exiters e
INNER JOIN withdrawers w ON e.person_id = w.person_id
INNER JOIN people p ON p.id = e.person_id
INNER JOIN callers c ON c.caller = p.phone_number;



-- It seems that we can get the calls with less than a minutes (duration < 60 if the duration in seconds)
with exiters as
(
    SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people p ON bsl.license_plate = p.license_plate
    WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
        AND minute BETWEEN 15 AND 25
),
withdrawers as
(
    SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
    FROM atm_transactions t
    JOIN bank_accounts a ON t.account_number = a.account_number
    JOIN people p ON p.id = a.person_id
    WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND year = 2023
        AND month = 7
        AND day = 28
),
callers as
(SELECT *
FROM phone_calls
WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
)
SELECT e.person_id, e.name, c.caller, c.receiver, c.duration, c.year, c.month, c.day
FROM exiters e
INNER JOIN withdrawers w ON e.person_id = w.person_id
INNER JOIN people p ON p.id = e.person_id
INNER JOIN callers c ON c.caller = p.phone_number;

-- WOW , only 2 left, Bruce and Diana, it seems challenging. Let's get the theif, his destination, and the accompile

--    In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--    The thief then asked the person on the other end of the phone to purchase the flight ticket.

--  I will not update my previous queries, I will start again

SELECT f.*, a.city origin_city
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
WHERE f.year = 2023
    AND f.month = 7
    AND f.day = 29 --the next day
ORDER BY hour, minute;

-- We need the first one
SELECT f.*, a.city origin_city
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
WHERE f.year = 2023
    AND f.month = 7
    AND f.day = 29 --the next day
ORDER BY hour, minute
LIMIT 1;

-- Let us get the passengers of this flight
SELECT passport_number
FROM passengers
WHERE flight_id IN (
            SELECT f.id
            FROM flights f
            JOIN airports a ON f.origin_airport_id = a.id
            WHERE f.year = 2023
                AND f.month = 7
                AND f.day = 29 --the next day
            ORDER BY hour, minute
            LIMIT 1
            );

-- ONLY 8 , which of the 2 suspected people is one of them
-- Lets get their person IDs

SELECT passport_number
FROM passengers
WHERE flight_id IN (
            SELECT f.id
            FROM flights f
            JOIN airports a ON f.origin_airport_id = a.id
            WHERE f.year = 2023
                AND f.month = 7
                AND f.day = 29 --the next day
            ORDER BY hour, minute
            LIMIT 1
            );

-- We forgot the destination of the flight
SELECT f.*, a.city origin_city, d.city destination_city
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
JOIN airports d ON f.destination_airport_id = d.id
WHERE f.year = 2023
    AND f.month = 7
    AND f.day = 29 --the next day
ORDER BY hour, minute
LIMIT 1;

-- The destination is New York City...
-- Lets see which one of those 2 has a flight ticket on this plane
-- I will update the query in a sepatate file and copy the final here to be easier

with exiters as
(
    SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people p ON bsl.license_plate = p.license_plate
    WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
        AND minute BETWEEN 15 AND 25
),
withdrawers as
(
    SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
    FROM atm_transactions t
    JOIN bank_accounts a ON t.account_number = a.account_number
    JOIN people p ON p.id = a.person_id
    WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND year = 2023
        AND month = 7
        AND day = 28
),
callers as
(SELECT *
FROM phone_calls
WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
),
flight as
(
SELECT f.*, a.city origin_city, d.city destination_city
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
JOIN airports d ON f.destination_airport_id = d.id
WHERE f.year = 2023
    AND f.month = 7
    AND f.day = 29 --the next day
ORDER BY hour, minute
LIMIT 1
),
travelers as
(
SELECT passport_number
FROM passengers
INNER JOIN flight ON flight.id = passengers.flight_id
)
SELECT e.person_id, e.name, c.caller, c.receiver, c.duration, c.year, c.month, c.day
FROM exiters e
INNER JOIN withdrawers w ON e.person_id = w.person_id
INNER JOIN people p ON p.id = e.person_id
INNER JOIN callers c ON c.caller = p.phone_number
INNER JOIN travelers t ON t.passport_number = p.passport_number;


-- Eurekaaaaaaaaaaaaaaaaaaaaaaaaaaa he is Bruce ... Lets get the other person and make the table more readable


with exiters as
(
    SELECT activity, p.id person_id, bsl.license_plate, name, hour, minute
    FROM bakery_security_logs bsl
    JOIN people p ON bsl.license_plate = p.license_plate
    WHERE activity = 'exit' AND year = 2023 AND month  = 7 AND day = 28 AND hour = 10
        AND minute BETWEEN 15 AND 25
),
withdrawers as
(
    SELECT a.person_id, p.name, t.account_number,t.atm_location, t.transaction_type
    FROM atm_transactions t
    JOIN bank_accounts a ON t.account_number = a.account_number
    JOIN people p ON p.id = a.person_id
    WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND year = 2023
        AND month = 7
        AND day = 28
),
callers as
(SELECT *
FROM phone_calls
WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
),
flight as
(
SELECT f.*, a.city origin_city, d.city destination_city
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
JOIN airports d ON f.destination_airport_id = d.id
WHERE f.year = 2023
    AND f.month = 7
    AND f.day = 29 --the next day
ORDER BY hour, minute
LIMIT 1
),
travelers as
(
SELECT passport_number, flight_id
FROM passengers
INNER JOIN flight ON flight.id = passengers.flight_id
)
SELECT e.person_id, e.name, c.caller, c.receiver, ac.name ACCOMPLICE, f.destination_city
FROM exiters e
INNER JOIN withdrawers w ON e.person_id = w.person_id
INNER JOIN people p ON p.id = e.person_id
INNER JOIN people ac ON c.receiver = ac.phone_number
INNER JOIN callers c ON c.caller = p.phone_number
INNER JOIN travelers t ON t.passport_number = p.passport_number
INNER JOIN flight f ON t.flight_id = f.id;

/*
+-----------+-------+----------------+----------------+------------+------------------+
| person_id | name  |     caller     |    receiver    | ACCOMPLICE | destination_city |
+-----------+-------+----------------+----------------+------------+------------------+
| 686048    | Bruce | (367) 555-5533 | (375) 555-8161 | Robin      | New York City    |
+-----------+-------+----------------+----------------+------------+------------------+
*/
