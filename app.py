from flask import Flask, render_template, request
import json
from datetime import datetime

app = Flask(__name__)
data_store = []

def get_av(x):
    return x['analogValue']

def get_po(x):
    return x['pumpOn'] * 1000

def get_ts(x):
    return str(x['timestamp'])

@app.route('/')
def index():
    av = list(map(get_av, data_store))
    po = list(map(get_po, data_store))
    ts = list(map(get_ts, data_store))
    chart_data = json.dumps(
        [
            {
                'y': av,
                'x': ts,
                'type': 'scatter'
            },
            {
                 'y': po,
                'x': ts,
                'type': 'scatter'
            }
         ])
    return render_template('index.html', chart_data=chart_data, db_data=data_store)

@app.route('/record', methods=['POST'])
def record():
    record = request.json
    record['timestamp'] = datetime.now()
    data_store.append(record)
    return record

if __name__ == '__main__':
    app.run()