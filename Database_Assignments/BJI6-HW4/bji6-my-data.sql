--Name: Ben Isenberg  Date: 2/20/13
--Username: BJI6


--Extra data
INSERT INTO PHARMACEUTICAL_CO
VALUES ('LIC1414', 'Bayer', 1111111111);

INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Drug A', 'Drug A Formula', 'LIC1414');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Drug B', 'Drug B Formula', 'LIC1414');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Drug C', 'Drug C Formula', 'LIC1414');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Drug D', 'Drug D Formula', 'LIC1414');
INSERT INTO DRUG (Name, Formula, Manufacturer_Registration)
VALUES ('Drug E', 'Drug E Formula', 'LIC1414');

INSERT INTO PATIENT
VALUES (555555555, 'Bob', 'C', 'Smith', '369 Fifth Avenue', date '1972-06-04', 1200, 'PA');
INSERT INTO PATIENT
VALUES (444444444, 'Yanni', 'C', 'Smith', '369 Fifth Avenue', date '1974-06-04', 1200, 'PA');


INSERT INTO PRESCRIPTION
VALUES (1200, 'PA', 555555555, 'Drug B', 11, date '2004-02-14', 1);
