    select original_title, premiered, cast(runtime_minutes as varchar) || " (mins)" from titles
    where genres like '%Sci-Fi%'
    order by runtime_minutes DESC
    limit 10