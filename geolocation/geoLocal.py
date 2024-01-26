import requests
import pandas as pd
import sqlite3
from time import sleep
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
    googlekey = "AIzaSyDab1KqlhwAEgwFP4FNrUMM1G3CNVpuBG8"
    params = {
        'address': address,
        'key': googlekey
    }
    url = 'https://maps.googleapis.com/maps/api/geocode/json'
    # Esecuzione della richiesta GET all'API di Google Maps
    try:
        response = requests.get(url, params=params)
        data = response.json()

        # Verifica se la richiesta è andata a buon fine
        if response.status_code == 200 and data['status'] == 'OK':
            # Estrazione delle coordinate geografiche
            location = data['results'][0]['geometry']['location']
            lat = location['lat']
            lng = location['lng']
            return lat, lng
        else:
            print("Errore nella richiesta all'API di Google Maps:", data['status'])
            return None, None
    except Exception as e:
        print("Errore durante la richiesta all'API di Google Maps:", e)
        return None, None

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
            googleMaps_LocatePlace(address)
            row['lat'] = lat
            row['long'] = long
        
        update_task(connection, placesToComplete)
        
    #c = connection.cursor()
    #print(c.execute("SELECT * FROM addresses").fetchall())

    #end
    connection.close()

    

