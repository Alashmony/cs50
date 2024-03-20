--List all movies released in 2010 and their ratings, in descending order by rating
SELECT DISTINCT title, rating
FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE m.year = 2010
ORDER BY rating DESC, title ASC;
