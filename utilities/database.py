import sqlite3
import random
import string
import datetime
import time

def create_database(db,table,inputs): #inputs is a string formated (user text,favorite_number int, time timestamp)
    conn = sqlite3.connect(db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # move cursor into database (allows us to execute commands)
    c.execute('''CREATE TABLE IF NOT EXISTS {} {};'''.format(table,inputs)) # run a CREATE TABLE command
    conn.commit() # commit commands
    conn.close() # close connection to database

def insert_into_database(db,table,values,dated = False):
    #c.execute('''INSERT into test_table VALUES (?,?);''',(user,favorite_of_user))
    value_slots = "(?)"
    for i in range(len(values)+dated-1):
        value_slots = value_slots[0:2] + ",?" + value_slots[2:]
    conn = sqlite3.connect(db)
    c = conn.cursor()
    if dated:
        c.execute('''INSERT into {} VALUES {};'''.format(table,value_slots),values+(datetime.datetime.now(),)) #with time
    else:
        c.execute('''INSERT into {} VALUES {};'''.format(table,value_slots),values)
    conn.commit()
    conn.close()

def delete_all_rows(db,table):
    """
    Delete all rows in the tasks table
    :param conn: Connection to the SQLite database
    :return:
    """
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute('''DELETE FROM {};'''.format(table))
    conn.commit()
    conn.close()

def lookup_database(db,table,delimiters="",col="*",delimiter_values = tuple()):
    #SELECT favorite_number FROM test_table;
    #SELECT * FROM test_table ORDER BY favorite_number ASC or DESC;
    #SELECT * FROM test_table WHERE favorite_number BETWEEN 1132 AND 1185;
    conn = sqlite3.connect(db)
    c = conn.cursor()
    print('''SELECT {} FROM {} {};'''.format(col,table,delimiters))
    things = c.execute('''SELECT {} FROM {} {};'''.format(col,table,delimiters),delimiter_values).fetchall()
    conn.commit()
    conn.close()
    return things