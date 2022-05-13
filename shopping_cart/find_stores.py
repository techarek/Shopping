import sqlite3
import requests
import sys
sys.path.append("__HOME__/final_project")
shopping_list = '__HOME__/final_project/.gitignore/shopping_list3.db'
#shopping_list = "shopping_list2.db"
import productInfo2 as info

def request_handler(request):
    if request['method'] == 'GET':
        if 'goal' not in request['args'] or 'lat' not in request['args'] or 'lon' not in request['args']:
            return "request must include goal, lat, and lon"
        goal = request['values']['goal']
        lat = float(request['values']['lat'])
        lon = float(request['values']['lon'])
        conn = sqlite3.connect(shopping_list)
        c = conn.cursor()
        c.execute(
            '''CREATE TABLE IF NOT EXISTS products (user text, item text, quantity int, UPC text, closest text, cheapest text);''')

        if goal == 'close':
            out = c.execute('''SELECT closest FROM products;''').fetchall()
            conn.commit()
            conn.close()
        elif goal == 'cheap':
            out = c.execute('''SELECT cheapest FROM products;''').fetchall()
            conn.commit()
            conn.close()
        else:
            conn.commit()
            conn.close()
            return "goal must be either cheap or close"
        out = [elm[0] for elm in out]
        return "".join(out)
    else:
        return "request must be a GET request"

#
#
# request = {'method':'GET', 'args':['goal','lat','lon'],'values':{'goal':'cheap','lat':0,'lon':0}}
# print(request_handler(request))
