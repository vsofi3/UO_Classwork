import unicodecsv as csv
data = []
with open("rawcountries.csv", 'r') as file:
    for line in file:
        rowData = []
        line = line.split(';')
        for col in line:
            rowData.append(col.rstrip())
        data.append(rowData)
with open("cleandata.csv", 'w') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerows(data)
