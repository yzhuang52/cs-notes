select p.name
from people as p
inner join (
    select distinct(c.person_id) as id
    from crew as c
    where c.title_id in (
    select c.title_id
    from crew as c
    inner join people as p on c.person_id = p.person_id and p.name == "Nicole Kidman" and p.born == 1967
    ) and (c.category == "actor" or c.category == "actress")
) on p.person_id = id
order by p.name asc;
