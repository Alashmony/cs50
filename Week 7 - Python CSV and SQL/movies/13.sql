-- List the names of all people who starred in a movie in which Kevin Bacon also starred
WITH kbm as
(
    SELECT movie_id FROM stars WHERE person_id IN
        (
            SELECT id FROM people WHERE birth = 1958 AND name = 'Kevin Bacon'
        )
)
SELECT DISTINCT name
FROM people p
INNER JOIN stars s ON s.person_id = p.id
INNER JOIN kbm ON kbm.movie_id = s.movie_id
WHERE p.name <> 'Kevin Bacon'
