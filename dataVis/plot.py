import matplotlib.pyplot as plot
xIndex = 0
yIndex = 0
xVals = []
yVals = []
data = []
with open("rawcountries.csv", 'r') as file:
    for line in file:
        i = 0
        rowData = []
        line = line.split(';')
        for col in line:
            if (col == "GDP"): #these two if statements set the index for x & y variables
                xIndex = i
            elif (col == "Infant mortality rate(deaths/1000 live births)"):
                yIndex = i
            rowData.append(col.rstrip())
            i += 1
        data.append(rowData)

for row in data[2:]:
    if (len(row[xIndex]) != 0) and (len(row[yIndex]) != 0):
        xVals.append((float(row[xIndex]) / 1000000000.0))
        yVals.append(float(row[yIndex]))
plot.scatter(xVals, yVals)
plot.show()
