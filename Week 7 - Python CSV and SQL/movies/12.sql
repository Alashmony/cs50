-- List the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred
-- Get Bradley Cooper ID in a CTE
/*WITH bc AS (
    SELECT id FROM people WHERE name = 'Bradley Cooper'
),
-- Get Jennifer Lawrence ID in a CTE
jl AS (
    SELECT id FROM people WHERE name = 'Jennifer Lawrence'
),*/
-- GET BC Movies
with bcm AS (
    SELECT movie_id FROM stars s WHERE person_id IN (SELECT id FROM people WHERE name = 'Bradley Cooper')
    ),
-- GET Jennifer Lawrence movies
jlm AS (
    SELECT movie_id FROM stars s WHERE person_id IN (SELECT id FROM people WHERE name = 'Jennifer Lawrence')
)
SELECT title
FROM movies
INNER JOIN bcm ON bcm.movie_id = movies.id
INNER JOIN jlm ON jlm.movie_id = movies.id
