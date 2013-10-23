# Comparing compound sort with multiple single sorts to see if they are equivalent

E.g. is ORDER BY a, b, c the same as ORDER BY c, then ORDER BY b, then ORDER by a?

Using the SQL Try-It editor from W3Schools with the Northwind database:

http://www.w3schools.com/sql/trysql.asp?filename=trysql_select_orderby2

### compound-sort.txt

SELECT Country, City, CustomerID  FROM Customers ORDER BY Country, City, CustomerID;

### multiple-single-sort.txt

SELECT Country, City, CustomerID  FROM (
  SELECT Country, City, CustomerID FROM (
    SELECT Country, City, CustomerID FROM Customers ORDER BY CustomerID
  ) ORDER BY City
) ORDER BY Country;

## Conclusion

With this data set, the two queries give the same result. Need to try with bigger data sets to confirm whether they are equivalent beyond reasonable doubt, or find someone who can confirm whether they are mathematically equivalent.

Sharon says they are.

One for Stackoverflow and the programmers list, methinks!