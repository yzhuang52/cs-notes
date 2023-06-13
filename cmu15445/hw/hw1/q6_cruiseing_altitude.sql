select t.primary_title, r.votes
from ratings as r 
inner join titles as t on t.title_id = r.title_id
inner join crew as c on t.title_id = c.title_id 
inner join people as p on c.person_id = p.person_id
where p.born = 1962 and p.name like "%Cruise%"
group by t.primary_title
order by r.votes desc 
limit 10;