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
