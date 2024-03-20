-- List the names of all people who have directed a movie that received a rating of at least 9.0
/*SELECT count(DISTINCT name)
FROM people p
JOIN directors d ON p.id = d.person_id
JOIN movies m ON d.movie_id = m.id
JOIN ratings r ON m.id = r.movie_id
WHERE r.rating >= 9*/
SELECT name
FROM people p
WHERE p.id IN
(
    SELECT person_id
    FROM directors
    WHERE movie_id IN
    (
        SELECT movie_id
        FROM ratings
        WHERE rating >= 9
    )
)
