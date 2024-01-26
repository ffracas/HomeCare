import requests
import pandas as pd
import sqlite3
from geopy.geocoders import ArcGIS

def connect_db() :
    try:
        return sqlite3.connect('example.db')
    
    except sqlite3.Error as e:
        print("Errore durante la connessione al database:", e)    

def getPan(conn) :
    return pd.read_sql_query('SELECT * FROM addresses WHERE lat IS NULL OR long IS NULL', conn)

#def getListOfPlacesToComplete(db_cursor) :
#    return db_cursor.execute('SELECT * FROM addresses WHERE lat IS NULL OR long IS NULL')

def googleMaps_LocatePlace (address: str):
    #googlekey = "AIzaSyDab1KqlhwAEgwFP4FNrUMM1G3CNVpuBG8"
    #response = requests.get('https://maps.googleapis.com/maps/api/geocode/json?address=1600+Amphitheatre+Parkway,+Mountain+View,+CA')
    #resp_json_payload = response.json()
    #print(resp_json_payload['results'][0]['geometry']['location'])
    print("hello")

def ArcGIS_LocatePlace (address: str) :
    finder = ArcGIS()
    location = finder.geocode(address)
    return location.latitude, location.longitude

def update_task(conn, task):
    sql = ''' UPDATE addresses
              SET lat = ? , long = ? 
              WHERE patient = ?'''
    cur = conn.cursor()
    for _, data in task.iterrows():
        geoCoord = (data['lat'], data['long'], data['patient'])
        cur.execute(sql, geoCoord)
        conn.commit()

if __name__ == "__main__":
    connection = connect_db()
    placesToComplete = getPan(connection)       #getListOfPlacesToComplete(connection.cursor())
    #address = "Via dell'Università 50, Cesena, Italia"
    if not placesToComplete.empty :
        for index, row in placesToComplete.iterrows():
            address = "{}, {}, {}, {}, {}".format(row['number'], row['road'], row['city'], row['cap'], row['country'])
            lat, long = ArcGIS_LocatePlace(address)
            row['lat'] = lat
            row['long'] = long
        
        update_task(connection, placesToComplete)
        #placesToComplete.to_sql('addresses', connection, if_exists='append', index=False)
    c = connection.cursor()
    print(c.execute("SELECT * FROM addresses").fetchall())

    #end
    connection.close()

    

