select cast((t.premiered/10)*10 as varchar) || "s" as decade, round(avg(r.rating), 2) as avg_rating, max(r.rating), min(r.rating), count(*)
from ratings as r join titles as t 
on r.title_id = t.title_id
where t.premiered is not NULL
group by decade
order by avg_rating desc, decade asc;