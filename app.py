from flask import Flask, render_template

import plotly
import plotly.graph_objs as go

import pandas as pd
import numpy as np
import json

app = Flask(__name__)

def create_plot():
    N = 40
    x = np.linspace(0, 1, N)
    y = np.random.randn(N)
    df = pd.DataFrame({'x': x, 'y': y}) 

    data = [
        go.Bar(
            x=df['x'],
            y=df['y']
        )
    ]

    graphJSON = json.dumps(data, cls=plotly.utils.PlotlyJSONEncoder)

    return graphJSON

@app.route('/')
def index():
    bar = create_plot()
    chart_data = json.dumps(
        [{'y': [2, 2, 3, 5],
         'x': [1, 2, 3, 4],
         'type': 'scatter'}])
    return render_template('index.html', data=chart_data)

if __name__ == '__main__':
    app.run()