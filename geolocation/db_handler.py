import sqlite3

conn = sqlite3.connect('example.db')

tableName = ('addresses', )

c = conn.cursor()
# Create table
c.execute('''CREATE TABLE IF NOT EXISTS addresses (patient text PRIMARY KEY, road text NOT NULL, 
                                    number text NOT NULL, cap text NOT NULL, city text NOT NULL, 
                                    country text NOT NULL, lat real DEFAULT NULL, long real DEFAULT NULL)''')
# Insert data
addresses = [("p0", "via delle madonne", "12a", "75012", "Bernalda", "Italia", 12.23456, -12.435345),
             ("p1", "via dell'università", "50", "47522", "Cesena", "Italia", 12.23456, -12.435345),
             ("p2", "Via dell'università", "50", "47522", "Cesena", "Italia", None, None)]
c.executemany('INSERT INTO addresses VALUES (?, ?, ?, ?, ?, ?, ?, ?)', addresses)
conn.commit()

for row in c.execute('SELECT * FROM addresses WHERE lat IS NULL OR long IS NULL'):  #WHERE lat = NULL OR long = NULL
    print(row)

c.close()