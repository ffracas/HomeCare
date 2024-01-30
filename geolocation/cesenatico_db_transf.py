import sqlite3
import pandas as pd
import re
import math

conn_new_db = sqlite3.connect('unicdata.db')
conn_old_db = sqlite3.connect('cesenatico.sqlite')

df = pd.read_sql_query("SELECT * from cesenatico", conn_old_db)
c = conn_old_db.cursor()
print(c.execute("SELECT Indirizzo_di_domicilio, Comune_di_domicilio from cesenatico where PK_UID = 44").fetchall())

c = conn_new_db.cursor()
# Create table
c.execute('''CREATE TABLE IF NOT EXISTS addresses (patient text PRIMARY KEY, road text NOT NULL, 
                                    number text DEFAULT 2, cap text NOT NULL, city text NOT NULL, 
                                    country text DEFAULT Italia, lat real DEFAULT NULL, long real DEFAULT NULL)''')
# Insert data  altri campi: 'DISTRETTO' 'unita' 'tipologia_di_intervento'  'Localita_di_domicilio' 

addresses = []
for i in range(len(df.index)):
    road = re.sub("\sINT[._]|\sNR[._]|\sN[._]|\s[(]\s*[)]|\s{2,}", "", df.loc[i, 'Indirizzo_di_domicilio'])
    #road = df.loc[i, 'Indirizzo_di_domicilio']
    cap = None if (math.isnan(df.loc[i, 'cap_domicilio'])) else int(df.loc[i, 'cap_domicilio'])
    row = ("pat{}".format(i), road, df.loc[i, 'num'], cap, df.loc[i, 'Comune_di_domicilio'], "Italia", None, None )
    addresses.append(row)
    print(row)
#c.executemany('INSERT INTO addresses VALUES (?, ?, ?, ?, ?, ?, ?, ?)', addresses)
conn_new_db.commit()

for row in c.execute('SELECT * FROM addresses WHERE lat IS NULL OR long IS NULL'):  #WHERE lat = NULL OR long = NULL
    print(row)

c.close()