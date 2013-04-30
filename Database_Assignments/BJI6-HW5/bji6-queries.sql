--Name: Ben Isenberg  Date: 2/27/13
--Username: BJI6

----------------------- Question 2.1
select count(P.D_FullName), sum(P.Quantity)
from PADoctors P
group by P.D_FullName;

----------------------- Question 2.2
create table Temp as
select P.P_FullName, P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month, P.Doctor_Lic_No
from MultipleDoctors P ;

create table Temp2 as
select P.P_FullName, P.Drug, P.Year, P.Month, P.Doctor_Lic_No
from Temp P
where P.Drug in
(select T.Drug 
from Temp T
group by T.Drug
having count(*) > 1)
and P.Year in
(select T.Year 
from Temp T
group by T.Year
having count(*) > 1)
and P.Month in
(select T.Month 
from Temp T
group by T.Month
having count(*) > 1);

select T.P_FullName
from Temp2 T
where T.P_FullName in
(select P.P_FullName
from Temp2 P
group by P.P_FullName
having count(*) > 1)
and T.Doctor_Lic_No in
(select P.Doctor_Lic_No
from Temp2 P
group by P.Doctor_Lic_No
having count(*) = 1);

drop table Temp;
drop table Temp2;
purge recyclebin;