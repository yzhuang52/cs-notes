select p.name, count(*) as freq
from crew as c join people as p 
on c.person_id = p.person_id
group by p.name
order by freq desc
limit 20;