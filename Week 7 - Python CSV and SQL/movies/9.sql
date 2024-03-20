-- List the names of all people who starred in a movie released in 2004, ordered by birth year
/*SELECT COUNT(DISTINCT name)
FROM people p
JOIN stars s ON p.id = s.person_id
JOIN movies m ON s.movie_id = m.id
WHERE m.year = 2004
ORDER BY birth;
*/
SELECT name
FROM people
WHERE id in
    (
        SELECT person_id
        FROM stars
        WHERE movie_id in
        (
            SELECT id
            FROM movies
            WHERE year = 2004
        )
    )
ORDER BY birth;
