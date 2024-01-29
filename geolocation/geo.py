import requests

def openstreetmap_geocode(address):
    # Costruzione dell'URL per la richiesta all'API di OpenStreetMap Nominatim
    url = 'https://nominatim.openstreetmap.org/search'
    params = {
        'q': address,
        'format': 'json',
        'limit': 1
    }

    # Esecuzione della richiesta GET all'API di OpenStreetMap Nominatim
    try:
        response = requests.get(url, params=params)
        data = response.json()

        # Verifica se la richiesta è andata a buon fine
        if response.status_code == 200 and data:
            # Estrazione delle coordinate geografiche
            lat = float(data[0]['lat'])
            lng = float(data[0]['lon'])
            return lat, lng
        else:
            print("Errore nella richiesta all'API di OpenStreetMap Nominatim")
            return None, None
    except Exception as e:
        print("Errore durante la richiesta all'API di OpenStreetMap Nominatim:", e)
        return None, None

def googleMaps_locatePlace (address: str):
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
    
if __name__ == "__main__":
    address = ["50 via dell'università, Cesena, 47522, Italia", "20 via dell'università, Cesena, 47522, Italia"]
    results = []
    # Chiamata alla funzione di geocodifica di Google
    for add in address:
        lat, lng = openstreetmap_geocode(add) #googleMaps_locatePlace(address)
        results.append((lat, lng))


    for data in results:
        print(data)
    # Stampa delle coordinate geografiche ottenute
    '''if lat is not None and lng is not None:
        print("Coordinate geografiche per l'indirizzo '{}':".format(address))
        print("Latitudine:", lat)
        print("Longitudine:", lng)
    else:
        print("Impossibile geocodificare l'indirizzo:", address)'''