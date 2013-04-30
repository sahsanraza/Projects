--Name: Ben Isenberg  Date: 2/20/13
--Username: BJI6

--Display tables from assignment
SELECT * FROM contract ;
SELECT * FROM sell ;
SELECT * FROM prescription ;
SELECT * FROM drug ;
SELECT * FROM pharmacy ;
SELECT * FROM pharmaceutical_co ;
SELECT * FROM patient ;
SELECT * FROM doctor ;


-----------------------------------------------------------------------
-- Question #4

--Query 4.1
SELECT DRUG.Name 
FROM DRUG, PHARMACEUTICAL_CO 
WHERE DRUG.Manufacturer_Registration = PHARMACEUTICAL_CO.PCM_Registration AND PHARMACEUTICAL_CO.Name = 'Bayer';

--Query 4.2
CREATE TABLE Docs 
AS (SELECT DISTINCT DOCTOR.FirstName || DOCTOR.MiddleName || DOCTOR.LastName AS FullName, DOCTOR.Doctor_Lic_No, DOCTOR.Doctor_Lic_State
FROM DOCTOR, PATIENT, PRESCRIPTION
WHERE PATIENT.FirstName = 'Bob' AND PATIENT.LastName = 'Smith' AND PRESCRIPTION.Patient = PATIENT.SSN AND DOCTOR.Doctor_Lic_No = PRESCRIPTION.Doctor_Lic_No AND DOCTOR.Doctor_Lic_State = PRESCRIPTION.Doctor_Lic_State );

--Query 4.3 
CREATE TABLE Temp1 
AS (SELECT * 
FROM DRUG D, PRESCRIPTION PRE 
WHERE PRE.Drug = D.Name);
SELECT P.SSN, COUNT(Temp1.Name)
FROM PATIENT P LEFT OUTER JOIN Temp1 ON P.SSN = Temp1.Patient
GROUP BY P.SSN;
DROP TABLE Temp1;

--Query 4.4 --Just lists max number not company name :(
SELECT MAX(Num) 
FROM (SELECT COUNT(P.Drug) AS Num 
FROM DRUG D, PRESCRIPTION P
WHERE D.Name = P.Drug
GROUP BY D.Name);

--Query 4.5
SELECT PC.Name, COUNT(C.Store)
FROM PHARMACEUTICAL_CO PC, CONTRACT C
WHERE PC.PCM_Registration = C.Manufacturer_Registration
GROUP BY PC.Name;


-----------------------------------------------------------------------
-- Question #5

--Transaction 5.1
SET TRANSACTION READ WRITE NAME 'TEST' ;
SET CONSTRAINTS ALL DEFERRED;
UPDATE PHARMACY P
SET P.Name='CVS'
WHERE P.Name='Revco';
UPDATE SELL S
SET S.Store='CVS'
WHERE S.Store='Revco';
UPDATE CONTRACT C
SET C.Store='CVS'
WHERE C.Store='Revco';
COMMIT;

--Transaction 5.2
SET TRANSACTION READ WRITE NAME 'TEST2' ;
SET CONSTRAINTS ALL DEFERRED;
INSERT INTO PRESCRIPTION
VALUES (12345, 'PA', 123346789, 'Cloraseptic', 0, date '2012-12-12', 1);
INSERT INTO PATIENT
VALUES (123346789, 'Alice', 'J', 'Bawer', '124 Liberty Ave, Pittsburgh, PA 15213', date '1990-03-03', 12345, 'PA');
UPDATE PRESCRIPTION P
SET P.Quantity = 1
WHERE P.Patient = 123346789;
--Extra data needed for referential integrity
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Cloraseptic', 'Clora Formula', 'LIC2019');
INSERT INTO DOCTOR
VALUES (12345, 'PA', 'John', 'J.', 'Adams', 'Cardiology', 9);
COMMIT;



