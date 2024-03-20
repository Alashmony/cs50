--Determine the average rating of all movies released in 2012
SELECT AVG(rating)
FROM ratings r
JOIN movies m ON m.id = r.movie_id
WHERE m.year = 2012;
