from collections import Counter
import os
import csv
import sqlite3

from flask import Flask, request, g
from nta import NaverTalkApi, NaverTalkApiError, NaverTalkApiConnectionError

from OpenSSL import SSL
context = SSL.Context(SSL.SSLv3_METHOD)
cert = 'future.crt'
pkey = 'future.key'
context.use_privatekey_file(pkey)
context.use_cerfiticate_file(cert)

#NAVER_TALK_SECRET = os.environ['NAVER_TALK_SECRET']
#ntalk = NaverTalkApi(NAVER_TALK_SECRET)
    

DATABASE = '/var/www/html/flaskapp/natlpark.db'



app = Flask(__name__)
app.config.from_object(__name__)

@app.route('/naver', methods=['POST'])
def app_enterance():
    print("*" * 40)
    req = request.get_data(as_text=True)
    print('* Recieved Data:')
    print(req)
    try:
        ntalk.webhook_handler(req)
    except NaverTalkApiError as e:
        print(e)
    except NaverTalkApiConnectionError as e:
        print(e)
    print("*" * 40)

    return "ok"



def connect_to_database():

    return sqlite3.connect(app.config['DATABASE'])



def get_db():
    db = getattr(g, 'db', None)
    if db is None:
        db = g.db = connect_to_database()

    return db



@app.teardown_appcontext

def close_connection(exception):

    db = getattr(g, 'db', None)

    if db is not None:

        db.close()



def execute_query(query, args=()):

    cur = get_db().execute(query, args)

    rows = cur.fetchall()

    cur.close()

    return rows



@app.route("/viewdb")

def viewdb():
    rows = execute_query("""SELECT * FROM natlpark""")
    return '<br>'.join(str(row) for row in rows)



@app.route("/state/<state>")

def state_query(state):

    rows = execute_query("""SELECT * FROM natlpark WHERE state = ?""",

                         [state.title()])

    return '<br>'.join(str(row) for row in rows)

@app.route('/')
def hello_world():

    return 'hello'




@app.route('/update')
def update_db():
    data=request.values['data']
   # cur=get_db().cursor()
   # conn=sqlite3.connect('natlpark.db')
#    cur=conn.cursor()

    execute_query("""INSERT INTO natlpark(area) VALUES(?)""",[data])
    get_db().commit()
    rows = execute_query("""SELECT * FROM natlpark""")
    return '<br>'.join(str(row) for row in rows)
   	


@app.route('/countme/<in_str>')

def count_me(in_str):

    return '<br>'.join('"{}": {}'.format(let, cnt)

                       for let, cnt in Counter(in_str).most_common())





if __name__ == '__main__':

    app.run(ssl_context=(cert, pkey))
