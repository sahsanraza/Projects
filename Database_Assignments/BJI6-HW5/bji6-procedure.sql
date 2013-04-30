--Name: Ben Isenberg  Date: 2/27/13
--Username: BJI6

------------------------- Question 4.1  --compilation errors but the sql statements work outside the function
-- cant create tables inside functions :(
create or replace function FindAvailableDoctor ( Doc_State in varchar2 ) 
	return number
is
	Doc_Lic number(10);
begin
	create table Temp as
	(select DOCTOR.Doctor_Lic_No, PATIENT.SSN
	from DOCTOR, PATIENT
	where DOCTOR.Doctor_Lic_State = Doc_State and DOCTOR.Doctor_Lic_No = PATIENT.PCP_Lic_No);
	create table Temp2 as
	(select Temp.Doctor_Lic_No, count(*) as counter
	from Temp
	group by Temp.Doctor_Lic_No);
	select Temp2.Doctor_Lic_No into Doc_Lic
	from Temp2
	where counter = (select min(counter) from Temp2);
	drop table temp;
	drop table temp2;
	purge recyclebin;
	return Doc_Lic;
end;
/

------------------------- Question 4.2
create or replace trigger OnDeleteDoctor
after delete on DOCTOR
for each row
begin
update PRESCRIPTION
set PRESCRIPTION.Doctor_Lic_No = -1
where PRESCRIPTION.Doctor_Lic_No = :old.Doctor_Lic_No;
update PRESCRIPTION
set PRESCRIPTION.Doctor_Lic_State = 'XX'
where PRESCRIPTION.Doctor_Lic_State = :old.Doctor_Lic_State;
update PATIENT
set PATIENT.PCP_Lic_No = -1
where PATIENT.PCP_Lic_No = :old.Doctor_Lic_No;
update PATIENT
set PATIENT.PCP_Lic_State = 'XX'
where PATIENT.PCP_Lic_State = :old.Doctor_Lic_State;
end;
/

