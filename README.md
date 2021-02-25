# Quick and Dirty Monitor for Offerzen Make Day Smart Plant

Keep track of what your smart plant is up to while you're not watching.

## What is it?

Have your ESP8266 send updates to a simple python Flask web app that will serve you a graph that looks like this!

![Monitoring Plot](resources/first_plot.png)

## Background

Quickly hacked this together after building a self watering plant with Offerzen's Make day https://github.com/OfferZen-Make/plant_tech_ams

## Instructions

### Run the server locally

Assuming you have python installed (TODO: add instructions)

```
pip install -r requirements.txt
python app.py
```
Then go check out a blank plot at `http://localhost:5000`

### Test it out

With your server running locally on port `5000` (the default), run in a terminal

```
curl --header "Content-Type: application/json" \
  --request POST \
  --data '{"analogData": 834, "pumpOn": false}' \
  http://localhost:5000/record
```
Maybe do it a few times. Then go reload your page at `http://localhost:5000`

### Host on Heroku

TODO: add proper instructions

In the meantime, follow this https://devcenter.heroku.com/articles/getting-started-with-python except clone this repo instead of their one

### Get your smart plant to send you updates

Upload the sketch at `esp8266/smart_plant` (add in your ssid and password first). You also need to add the URL for your web app (whatever it is called on heroku). This is just an evolution of the sketches that are in the Make repo linked above so get those working first.

## Limitations

HA! Lots. Pretty much everything. Here are some highlights

- The data store is just in memory so everything is lost whenever the process that runs your server restarts
- The plot just shows you everything and you have no control over that
- You have to send exactly the data that the server is expecting otherwise it won't know what to do with it (have a look at the sketch in `sdp8266/smart_plant` to see what it sends)