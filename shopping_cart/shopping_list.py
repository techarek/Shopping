import sys
import sqlite3
sys.path.append("__HOME__/final_project")
import requests
import productInfo2 as info

shopping_list = '__HOME__/final_project/.gitignore/shopping_list3.db'
#shopping_list = "shopping.db"

def request_handler(request):
    if request['method'] == 'POST':
        added_item = request['form']['item'].strip()
        user = request['form']['user']
        lat = float(request['form']['lat'])
        lon = float(request['form']['lon'])
        quantity = int(request['form']['quantity'])
        UPC = request['form']['UPC']
        limit = 5000

        if added_item == "":
            return "Invalid Item"

        conn = sqlite3.connect(shopping_list)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS products (user text, item text, quantity int, UPC text, closest text, cheapest text);''')

        all_items = c.execute('''SELECT item, quantity, UPC FROM products;''').fetchall()

        shopping_list_info = {}  # maps item name to dict mapping stores to cost

        try:
            item_name, number = c.execute('''SELECT item, quantity FROM products WHERE user = (?) AND item = (?);''', (user, added_item)).fetchall()[0]
            if number + quantity <= 0:
                c.execute('''DELETE from products WHERE user = (?) AND item = (?)''', (user, added_item))
                conn.commit()
                conn.close()
                return "Deleted"
            else:
                quantity = number + quantity
                # c.execute('''UPDATE products SET quantity = (?) WHERE user = (?) AND item = (?)''', (number + quantity, user, added_item))
        except:
            if quantity > 0:
                pass
                # c.execute('''INSERT into products VALUES (?, ?, ?,?)''', (user, added_item, quantity, UPC))

        shopping_list_info = {}  # maps item name to dict mapping stores to cost
        if UPC != "":
            try:
                data = info.getProduct(UPC)
            except:
                # print("Couldn't find UPC")
                data = info.getProduct(added_item)
        else:
            data = info.getProduct(added_item)  # returns dict mapping stores to cost
        for store in data:
            data[store] = quantity * float(data[store])
        shopping_list_info[added_item] = data

        # CHEAP/CLOSE
        cheap_stores = set()
        key = "fIFX64ocZGSMxfmbuSy9i1n4bGFd9o1r"
        stores = set()
        min_distance = float('inf')
        min_dist = ""
        for item in shopping_list_info:
            min_cost = float('inf')
            min_store = ''
            for store in shopping_list_info[item]:
                cur_cost = float(shopping_list_info[item][store])
                temp = None
                if store and not "." in store:
                    store.replace(" ", "%20")
                    url = """https://api.tomtom.com/search/2/nearbySearch/.json?lat={}&lon={}&countrySet=US&idxSet=POI&brandSet={}&key={}""".format(
                        lat, lon, store, key)
                    r = requests.get(url)
                    response = r.json()
                    if response["results"]:
                        temp = response["results"][0]["dist"]
                        if response["results"][0]["dist"] < min_distance:
                            min_distance = response["results"][0]["dist"]
                            min_dist = store
                if cur_cost < min_cost:
                    if store and not "." in store:
                        if temp is not None:
                            if temp < limit:
                                min_cost = cur_cost
                                min_store = store
                    else:
                        min_cost = cur_cost
                        min_store = store
            if min_dist:
                stores.add(min_dist)
            if not min_store == '':
                cheap_stores.add(min_store)
        cheapest = ""
        for st in cheap_stores:
            cheapest += st + "\n"
        closest = ""
        for st in stores:
            closest += st + "\n"
        
        try:
            c.execute('''SELECT item, quantity FROM products WHERE user = (?) AND item = (?);''', (user, added_item)).fetchall()[0]
            c.execute('''UPDATE products SET quantity = (?) WHERE user = (?) AND item = (?)''', (quantity, user, added_item))
        except:
            c.execute('''INSERT into products VALUES (?, ?, ?,?,?,?)''', (user, added_item, quantity, UPC, closest, cheapest))
        conn.commit()
        conn.close()

        return "Completed"

    elif request["method"] == "GET":
        user = request['values']['user']
        conn = sqlite3.connect(shopping_list)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS products (user text, item text, quantity int, UPC text, closest text, cheapest text);''')

        all_items = c.execute('''SELECT item, quantity, UPC FROM products WHERE user = (?);''',(user,)).fetchall()
        conn.commit()
        conn.close()

        items_string = ""
        for item in all_items:
            if item[2] == "":
                items_string += "{}&None&{};".format(item[1], item[0])
            else:
                items_string += "{}&{}&{};".format(item[1], item[2], item[0])

        return items_string
