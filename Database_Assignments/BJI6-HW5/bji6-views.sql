--Name: Ben Isenberg  Date: 2/27/13
--Username: BJI6

------------------------- Question 1.1 
-- I assume that the last 3 years are 2013, 2012, 2011
create view PADoctors as
(select D.FirstName || D.MiddleName || D.LastName as D_FullName, D.Doctor_Lic_No, 
P.FirstName || P.MiddleName || P.LastName as P_FullName, P.SSN,
PRE.PrescDate, PRE.Drug, PRE.Quantity  
from DOCTOR D, PATIENT P, PRESCRIPTION PRE 
where D.Doctor_Lic_State = 'PA'
and D.Doctor_Lic_No = PRE.Doctor_Lic_No and P.SSN = PRE.Patient and PRE.PrescDate > date '2010-12-31')
order by D_FullName ;


------------------------- Question 1.2
create table Temp as
select P.Patient
from PRESCRIPTION P
group by P.Patient
having count(P.Patient) > 1;

create table Temp2 as
select distinct PRE.Doctor_Lic_No, PRE.Patient, PRE.Doctor_Lic_State
from PRESCRIPTION PRE, Temp T
where PRE.Patient = T.Patient ;

create table Temp3 as
select T.Patient
from Temp2 T
group by T.Patient
having count(T.Patient) > 1 ;

create table Temp4 as
select T2.Patient, T2.Doctor_Lic_No, T2.Doctor_Lic_State
from Temp2 T2, Temp3 T3
where T3.Patient = T2.Patient ; 

create table Temp5 as
select T4.Patient, T4.Doctor_Lic_No, T4.Doctor_Lic_State, PRE.Drug, PRE.Quantity, PRE.PrescDate
from Temp4 T4, PRESCRIPTION PRE
where T4.Patient = PRE.Patient and T4.Doctor_Lic_No = PRE.Doctor_Lic_No and T4.Doctor_Lic_State = PRE.Doctor_Lic_State ;

create view MultipleDoctors as
select D.FirstName || D.MiddleName || D.LastName as D_FullName,
D.Doctor_Lic_No, D.Doctor_Lic_State, P.FirstName || P.MiddleName || P.LastName as P_FullName,
P.SSN, P.PCP_Lic_No, P.PCP_Lic_State, PRE.Drug, PRE.Quantity, PRE.PrescDate, D.Formula
from DOCTOR D, PATIENT P, DRUG D, Temp5 T5, PRESCRIPTION PRE
where D.Doctor_Lic_No = T5.Doctor_Lic_No and D.Doctor_Lic_State = T5.Doctor_Lic_State 
and P.SSN = T5.Patient and D.Name = T5.Drug and PRE.PrescDate = T5.PrescDate and PRE.Drug = T5.Drug
and PRE.Quantity = T5.Quantity;

drop table Temp;
drop table Temp2;
drop table Temp3;
drop table Temp4;
purge recyclebin;