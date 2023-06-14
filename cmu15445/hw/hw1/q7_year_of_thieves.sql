select count(*)
from titles
where premiered = (
    select premiered
    from titles
    where primary_title = "Army of Thieves"
);