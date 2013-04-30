--Name: Ben Isenberg  Date: 2/27/13
--Username: BJI6

--------------------- Question 3.1
create materialized view PADoctors_MV
build immediate refresh complete on demand
as select *
from PADoctors ;

--------------------- Question 3.2
insert into PRESCRIPTION values (1203, 'PA', 546746535, 'Aspirin', 1, date '2012-01-01');
commit;


------------------------ Question 3.3
--The queries produce the same output because nothing has changed in the tables of the database since making
--the two views.
------ Q1
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;

------ Q2
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors_MV P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;


------------------------ Question 3.4
--I still get the same output for both views after refreshing.
execute DBMS_MVIEW.REFRESH('PADoctors_MV') ;

------ Q1
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;

------ Q2
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors_MV P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;

------------------------ Question 3.5
--The timing of the first query is 00:00:00.24 and the second is 00:00:00.21 

set timing on;
------ Q1
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;

set timing off;

set timing on;

------ Q2
create table Temp as
select P.Drug, extract(year from P.PrescDate) as Year, extract(month from P.PrescDate) as Month
from PADoctors_MV P
where extract(year from P.PrescDate) = '2012';

create table Temp2 as
select T.Drug, T.Month, count(*) as maxDrug
from Temp T
group by T.Month, T.Drug
order by T.Month;

create table Temp3 as
select T.Month, max(maxDrug) as maxx
from Temp2 T
group by T.Month;

select T2.Drug, T2.Month, T3.maxx
from Temp2 T2, Temp3 T3
where T2.maxDrug = T3.maxx and T2.Month = T3.Month
order by T2.Month, T3.maxx;

drop table temp;
drop table temp2;
drop table temp3;
purge recyclebin;

set timing off;