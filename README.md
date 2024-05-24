# Motion Based Authentication

This project is designed as CA2 of the Cyber Physics Systems at the University of Tehran, Spring 2024.

In this project, we designed a motion based authentication app for mobile devices that is able to set a motion pattern as the answer and requiring to repeat that pattern for authentication.

This project consists of steps below:

- Front-end: mostly developed on QML and is capable of interacting with users. Assigned to **Pouya**
- Back-end: this part is consists of interacting with sensors, noise filtering and the logic of the app. Assigned to **Sina** and **Ali**.
- Analyzing performance: Using tools such as [Perfetto](https://ui.perfetto.dev/) to analysis proposes. Assigned to **Sepehr**
- Documentation and questions: Assigned to all members and it's been done by all of them.

---

## Front end

Our front-end is developed using QML and is capable of interacting with users. It's designed to be simple and user-friendly. It has several part, from top to bottom:

- In-line log: this part is designed to show the log of the app. It's designed to be simple and user-friendly. It can use different colors and designed reports authentication's result.
- Visualize patter: this part is designed to show the pattern that user should repeat. It has two *Graphical* and *Textual* mode, which can be changed by a slider.
- Control buttons: this part is designed to control the app. It has different buttons to interact with the app.

### Code structure

We can be divided the code into two parts: The Cpp codes, which are able to run the front-end and the QML codes, which are the main part of the front-end.
In the cpp part, we register the QML main entry in main.cpp and introduce the `MotionBasedAuth` object to the QML, this way it would be visible in the QML code.

```cpp
qmlRegisterType<MotionBasedAuth> ("MotionBasedAuthentication", 1, 0, "MotionBasedAuthentication");
...
const QUrl url(QStringLiteral("qrc:/MotionBasedAuthentication/Main.qml"));
```

The `MotionBasedAuth` is able to interact with the QML code using signals and slots. Q_Signals are used for sending signals to the QML and Q_Slots are used to ba called from the QML.

The main part is the QML codes. Our entry part is `Main.qml` which instantiates the ApplicationWindow component. Our main part in in `HomePage.qml`, which defines and place components of the application. It structures different parts and explained above. It also stablish a connection into back-end, the MotionBasedAuth class, used to control back and front interactions.

There exists another qml fine, named `PathDrawer.qml`. It contains a component used to draw a plot used in the Graphical view of the patters. This component is able to scale its input path. we used the default scale to $1000$ and the sensors values are low, but if the input pattern gets larger that the specified are, it would automatically scale it down to fit the specified area.

### Running the front-end

Here are some screenshots of the front-end:



```cpp


---

## Back-end

### Reading Sensors

### Noise filtering

### Pattern validation

---

## Perfetto

---
