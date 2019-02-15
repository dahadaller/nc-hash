By Tuesday, 02/05/2019 we needed to find a GUI framework that we can use to implement our application. This framework can be for web, desktop or mobile (iphone or android.) Whatever framework we choose, it must have the following qualities:

1. Lot's of documentation and a large developer network.
2. It must be able to interface with our obliv-C program (we have to be able to call C functions from our code)
3. The framework has to be capable of some html/css styling or some other form of theming. No Java Swing; it has to look good for demo purposes.
4. The framework has to be capable of accepting drag-and-drop input (or some intuitive design.) It has to be immediately intuitive to the user because we don't want them to have to fumble with the interface or read documentation in an emergency scenario.
Some additional considerations include:

- Choosing a language we can ask Skeith for advice on would be preferable, but not strictly necessary. 
- We should chose a langauage that is "resume buffer" i.e. an industry-standard framework used by tech companies in New York City.

## Mobile

### Xamarin (C#/C)

Visual Studio is a cross-platform IDE which, when combined with [Xamarin](https://docs.microsoft.com/en-us/xamarin/#pivot=platforms&panel=Cross-Platform), could allow us to develop a cross-platform native application on Mac, PC, Andoid and iOS. The development language is C#, which is very similar to Java and has C/C++ interoperability (it can call C functions directly.)

Problems with Xamarin inlcude:

- the only supported development environments are Mac and Windows (no Linux)
- I'm not sure if obliv-C will compile on a mobile device (have to ask Skeith about this)

### Android NDK (Java/C)

The [Android Native Development Kit](https://developer.android.com/ndk/guides/) (NDK) allows you to call C/C++ code from your Android Java application, while developing with [Android Studio](https://developer.android.com/studio/), which supports Mac, Linux and PC.

Problems with NDK are:

- not sure if obliv-C will compile on a mobile device.

### React Native (react.js/C)

React native allows you to use react.js to build mobile applications, instead of only web applications. React native can interact with C [through the JNI](https://thebhwgroup.com/blog/react-native-jni) (Java Native Interfacce). React native is not based on Node.js.

> **React Native** is implemented by a combination of Objective-C, Java, and JavaScript. When you use **React Native**, your JavaScript application code runs in WebKit/JavaScriptCore, not **Node**
>
> [source](https://www.quora.com/Why-is-React-Native-built-in-Node-js)

- again, not sure if obliv-c will work on mobile device



## Desktop

### Qt (Qt library/C++)

Qt is a cross-platform [you can mix C++ and C in the same code](https://stackoverflow.com/questions/16850992/call-a-c-function-from-c-code)  when compiling with gcc. Qt can also be made to look really nice too, the [telegram desktop client](https://github.com/telegramdesktop/tdesktop) was made with qt as was [KDE](https://www.kde.org/). Qt creator, the IDE we'd have to use (because Qt has it's own addition to the C++ compiler) works on Mac Linux and Windows.

- Qt seems like a really complicated framework to use

### GTK+ (C)

This is another cross-platform library, which, unlike Qt, works out of the box with C. The problem I've found with this framework is that I'm not sure how to keep consistency of interface across Linux distributions (re: [conflict between gnome and elementary os's HIG](https://medium.com/@alex285/7-things-elementary-os-does-terribly-wrong-in-2018-b9035df08791).) Theming is not uniform among Linux distribution.

### Nuklear(C)

[Nuklear](https://github.com/vurtun/nuklear) is a new, portable ANSI-C framework that makes it possible to make small, fast GUI's for C applications.

### Chromium Embedded Framework (C & another language)

[This framework](https://bitbucket.org/chromiumembedded/cef) is what works underneath electron apps. It can work with C/C++ along with a whole host of other languages. Essentially, this allows us to turn the project into a web-development project using react and bootstrap. This would be a great first step to porting the application to web-assembly. 

### Electron + Node.js (Javascript(node)/ C)

You can call c/c++ code from javascript with node.js [node-gyp tool](https://hackernoon.com/writing-cross-platform-c-is-easier-in-node-js-than-it-is-outside-of-node-js-a5c214f0cf10). Although this might be slower for a desktop app, it is easier to maintain than Qt and the installation process would be more uniform accross platforms.