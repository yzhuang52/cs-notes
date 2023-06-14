with people_1955 as (
    select person_id, name
    from people
    where born == 1955
),
rating_1955 as (
    select r.rating as rating, p_1955.name as name, p_1955.person_id as person_id
    from ratings as r
    join titles as t on r.title_id = t.title_id
    join crew as c on t.title_id = c.title_id
    join people_1955 as p_1955 on c.person_id = p_1955.person_id
    where t.type = "movie"
),
avg_rating as (
    select round(avg(rating),2) as rating, name
    from rating_1955
    group by person_id
),
decile_rating as (
    select rating, name,
    ntile(10) over (
        order by rating
    ) group_number 
    from avg_rating
)

select name, rating
from decile_rating
where group_number == 9
order by rating desc, name asc