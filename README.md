# ESP32-Server-Engine

A tool to generate ESP32 websites and APIs to control physical electronic components.

## esp32_sketch.ino

This is the baseline file that was used as a template. It creates a webserver on port 80 that has two routes:

- `/`: Serves the HTML form that a user can send requests through.
- `/control`: An API that's used to trigger a signal to electronic components and a server response to update the client.

## Generated file

The generated file is a blueprint of mostly what's needed to be done.

The generated file would make an ESP32-compatible webserver using the `index.html`, and the `config.json` files, the same files that were used/generated in the [Form-Engine](https://github.com/AbdullahAlKhafajiDev/Form-Engine).The **ESP32-Server-Engine** will take the same `config.json` that was used in the **Form-Engine** and spew out a template code for a backend server that can handle actions based on passed-in parameters.

Hyper-custom electronic behaviors will need to be coded in, but the generated file will get the user most of the way there, saving the user precious time to focus on the more complicated parts.

## Plan:

There will be a CLI **_Python_** program that will generate the Arduino C code by reading the `config.json` file.

**Note**: To make this experience more interactive, and less CLI-heavy, a user interface will be developed that will generate the `config.json` code, and generate the Arduino C code on the behalf of the user.
