with p as (
      select titles.primary_title as name, akas.title as dubbed
      from titles
      inner join akas on titles.title_id = akas.title_id
      where titles.primary_title = "House of the Dragon" AND titles.type = 'tvSeries'
      group by titles.primary_title, akas.title
      order by akas.title
),
c as (
      select row_number() over (order by p.name asc) as seqnum, p.dubbed as dubbed
      from p
),
flattened as (
      select seqnum, dubbed
      from c
      where seqnum = 1
      union all
      select c.seqnum, f.dubbed || ', ' || c.dubbed
      from c join
            flattened f
            on c.seqnum = f.seqnum + 1
)
select dubbed from flattened
order by seqnum desc limit 1;