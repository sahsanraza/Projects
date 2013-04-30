--Name: Ben Isenberg  Date: 2/17/13
--Username: BJI6

--Drop pre-existing tables if they exist
DROP TABLE contract CASCADE CONSTRAINTS;
DROP TABLE sell CASCADE CONSTRAINTS;
DROP TABLE prescription CASCADE CONSTRAINTS;
DROP TABLE drug CASCADE CONSTRAINTS;
DROP TABLE pharmacy CASCADE CONSTRAINTS;
DROP TABLE pharmaceutical_co CASCADE CONSTRAINTS;
DROP TABLE patient CASCADE CONSTRAINTS;
DROP TABLE doctor CASCADE CONSTRAINTS;
DROP TABLE docs CASCADE CONSTRAINTS;

--Empty recycle bin
PURGE RECYCLEBIN;

-----------------------------------------------------------------------
-- Question #1

create table DOCTOR
(
    Doctor_Lic_No number(10) not null,
    Doctor_Lic_State varchar2(10) not null,
    FirstName varchar2(15),
    MiddleName varchar2(15),
    LastName varchar2(20),
    Specialty varchar2(30),
    YearofExperience number(2),
    --Primary Key
    constraint Doctor_PK PRIMARY KEY(Doctor_Lic_No, Doctor_Lic_State) DEFERRABLE INITIALLY IMMEDIATE
);


create table PATIENT
(
    SSN number(9),
    FirstName varchar2(15),
    MiddleName varchar2(20),
    LastName varchar2(20),
    Address varchar2(64),
    DateOfBirth date,
    PCP_Lic_No number(10),
    PCP_Lic_State varchar2(10),
    --Primary Key
    constraint Patient_PK PRIMARY KEY(SSN) DEFERRABLE INITIALLY IMMEDIATE,
    --Foreign Key
    constraint Patient_FK FOREIGN KEY(PCP_Lic_No, PCP_Lic_State) REFERENCES DOCTOR(Doctor_Lic_No, Doctor_Lic_State) DEFERRABLE INITIALLY IMMEDIATE
);

create table PHARMACEUTICAL_CO
(
    PCM_Registration varchar2(20),
    Name varchar2(20),
    Phone number(10),
    --Primary key
    constraint PH_CO_PK PRIMARY KEY(PCM_Registration) DEFERRABLE INITIALLY IMMEDIATE
);

create table PHARMACY
(
    Name varchar2(20),
    Address varchar2(64) not null,
    Phone number(10) not null,
    --Primary key
    constraint PH_PK PRIMARY KEY(Name) DEFERRABLE INITIALLY IMMEDIATE,
    --Uniques
    constraint U_add unique(Address) DEFERRABLE INITIALLY IMMEDIATE,
    constraint U_phone unique(Phone) DEFERRABLE INITIALLY IMMEDIATE
);

create table DRUG
(
    Name varchar2(20),
    Formula varchar2(30),
    Manufacturer_Registration varchar2(20),
    --Primary key
    constraint D_PK PRIMARY KEY(Name) DEFERRABLE INITIALLY IMMEDIATE,
    --Foreign Key
    constraint D_FK FOREIGN KEY(Manufacturer_Registration) references PHARMACEUTICAL_CO(PCM_Registration) DEFERRABLE INITIALLY IMMEDIATE
);

create table PRESCRIPTION
(
    Doctor_Lic_No number(10),
    Doctor_Lic_State varchar2(10),
    Patient number(9),
    Drug varchar2(20),
    Quantity number(2),
    PrescDate date,
    --Primary Key
    constraint Pre_PK PRIMARY KEY(Doctor_Lic_No, Doctor_Lic_State, Patient, Drug, PrescDate) DEFERRABLE INITIALLY IMMEDIATE,
    --Foreign Key
    constraint Pre_FK1 FOREIGN KEY(Doctor_Lic_No, Doctor_Lic_State) references DOCTOR(Doctor_Lic_No, Doctor_Lic_State) DEFERRABLE INITIALLY IMMEDIATE,
    constraint Pre_FK2 FOREIGN KEY(Patient) references PATIENT(SSN) DEFERRABLE INITIALLY IMMEDIATE,
    constraint Pre_FK3 FOREIGN KEY(Drug) references DRUG(Name) DEFERRABLE INITIALLY IMMEDIATE
);

create table SELL
(
    Store varchar2(20),
    Drug varchar2(20),
    Price number(6,2),
    --Primary key
    constraint Sell_PK PRIMARY KEY(Store, Drug) DEFERRABLE INITIALLY IMMEDIATE,
    --Foreign keys
    constraint Sell_FK1 FOREIGN KEY(Store) references PHARMACY(Name) DEFERRABLE INITIALLY IMMEDIATE,
    constraint Sell_FK2 FOREIGN KEY(Drug) references DRUG(Name) DEFERRABLE INITIALLY IMMEDIATE
);

create table CONTRACT
(
    Store varchar2(20),
    Manufacturer_Registration varchar2(20),
    StartDate date,
    EndDate date,
    --Primary key
    constraint Con_PK PRIMARY KEY(Store, Manufacturer_Registration) DEFERRABLE INITIALLY IMMEDIATE,
    --Foreign keys
    constraint Con_FK1 FOREIGN KEY(Store) references PHARMACY(Name) DEFERRABLE INITIALLY IMMEDIATE,
    constraint Con_FK2 FOREIGN KEY(Manufacturer_Registration) references PHARMACEUTICAL_CO(PCM_Registration) DEFERRABLE INITIALLY
IMMEDIATE
);


-----------------------------------------------------------------------
-- Question #2

-- a
ALTER TABLE PRESCRIPTION
ADD Refill integer DEFAULT 1;

-- b
ALTER TABLE PRESCRIPTION
MODIFY Quantity DEFAULT 1;

-- c
ALTER TABLE DRUG
ADD SuggestMinPrice number(6,2);

-- d
ALTER TABLE SELL
ADD CHECK (Price >= 0);

-- e
ALTER TABLE PHARMACEUTICAL_CO
ADD CHECK (Name IS NOT NULL);


-----------------------------------------------------------------------
-- Question #3

--DOCTOR inserts
INSERT INTO DOCTOR
VALUES (1200, 'PA', 'John', 'K.', 'Steward', 'Internal Medicine', 10);
INSERT INTO DOCTOR
VALUES (1201, 'PA', 'Amber', 'T.', 'Calvin', 'Dermatology', 5);
INSERT INTO DOCTOR
VALUES (1202, 'OH', 'Alicia', 'A.', 'Johnson', 'Immunology', 20);
INSERT INTO DOCTOR
VALUES (1203, 'PA', 'Thomas', 'J.', 'Kurt', 'Ophthalmology', 15);
INSERT INTO DOCTOR (Doctor_Lic_No, Doctor_Lic_State, FirstName, LastName, Specialty, YearofExperience)
VALUES (1204, 'PA', 'Daniel', 'Robinson', 'Pediatrics', 9);

--Escape the & character
SET DEFINE OFF;

--PHARMACEUTICAL_CO inserts
INSERT INTO PHARMACEUTICAL_CO
VALUES ('LIC1234', 'Johnson&Johnson', 4122217889);
INSERT INTO PHARMACEUTICAL_CO
VALUES ('LIC2019', 'Pfizer', 6518900071);
INSERT INTO PHARMACEUTICAL_CO
VALUES ('LIC1904', 'Roche', 4518764902);

--PHARMACY inserts
INSERT INTO PHARMACY
VALUES ('Rite Aid', '318 5th Ave', 4124346603);
INSERT INTO PHARMACY
VALUES ('Revco', '242 5th Ave', 4125662619);
INSERT INTO PHARMACY
VALUES ('Mercy Health Center', '412 E.Commons', 4123234500);

--DRUG inserts
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Tylenol', 'Tylenol Formula', 'LIC1234');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Aspirin', 'Aspirin Formula', 'LIC2019');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('AspirinB', 'AspirinB Formula', 'LIC2019');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Motrin', 'Motrin Formula', 'LIC1904');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Allegra', 'Allegra Formula', 'LIC2019');

--SELL inserts
INSERT INTO SELL
VALUES ('Rite Aid', 'Tylenol', 8.29);
INSERT INTO SELL
VALUES ('Rite Aid', 'AspirinB', 3.00);

--CONTRACT inserts
INSERT INTO CONTRACT
VALUES ('Rite Aid', 'LIC1234', date '1990-01-01', date '2000-10-01');
INSERT INTO CONTRACT
VALUES ('Revco', 'LIC1904', date '2010-02-07', date '2020-01-01');

--PATIENT inserts
INSERT INTO PATIENT
VALUES (765765666, 'Michael', 'F', 'Cao', '320 Fifth Avenue', date '1940-01-01', 1203, 'PA');
INSERT INTO PATIENT
VALUES (888424247, 'Tom', 'K', 'Louis', '310 Fifth Avenue', date '1941-02-05', 1203, 'PA');
INSERT INTO PATIENT
VALUES (546746535, 'Bill', 'A', 'Newton', '330 Fifth Avenue', date '1940-12-11', 1203, 'PA');
INSERT INTO PATIENT
VALUES (246577767, 'Richard', 'F', 'Kates', '340 Fifth Avenue', date '1938-10-03', 1201, 'PA');
INSERT INTO PATIENT
VALUES (246345645, 'Mary', 'M', 'Missy', '350 Fifth Avenue', date '1944-03-04', 1200, 'PA');

--PRESCRIPTION inserts
INSERT INTO PRESCRIPTION
VALUES (1203, 'PA', 888424247, 'Tylenol', 20, date '2004-01-01', 3);
INSERT INTO PRESCRIPTION
VALUES (1200, 'PA', 765765666, 'Aspirin', 20, date '2004-01-03', 2);
INSERT INTO PRESCRIPTION
VALUES (1203, 'PA', 246577767, 'AspirinB', 20, date '2004-01-11', 1);
INSERT INTO PRESCRIPTION
VALUES (1202, 'OH', 546746535, 'Motrin', 10, date '2004-01-12', 1);
INSERT INTO PRESCRIPTION
VALUES (1201, 'PA', 765765666, 'Tylenol', 15, date '2004-01-14', 1);
INSERT INTO PRESCRIPTION
VALUES (1203, 'PA', 246577767, 'Allegra', 5, date '2004-01-23', 1);
INSERT INTO PRESCRIPTION
VALUES (1204, 'PA', 246345645, 'Motrin', 5, date '2004-01-13', 1);
INSERT INTO PRESCRIPTION
VALUES (1202, 'OH', 546746535, 'Aspirin', 10, date '2004-01-15', 1);
INSERT INTO PRESCRIPTION
VALUES (1200, 'PA', 246345645, 'Aspirin', 20, date '2004-01-09', 1);
INSERT INTO PRESCRIPTION
VALUES (1200, 'PA', 765765666, 'Aspirin', 10, date '2004-01-11', 1);


















