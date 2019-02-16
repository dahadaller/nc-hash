# JavaFX Demo

### Prerequisite reading

- A good place to start. https://www.tutorialspoint.com/javafx/
- I used this tutorial for software engineering project which was written entirely in Java and used JavaFX. https://code.makery.ch/library/javafx-tutorial/
- The GUI in the demo was based on this application. https://www.callicoder.com/javafx-css-tutorial/. This application was written by callicoder. Callicoder's github link is here https://github.com/callicoder/javafx-examples. He has other examples if you're interested. 

### Installation and what is required (windows)

#### What's required

- Download and install the newest version of Intellij. https://www.jetbrains.com/idea/

- Download and install Java 11 JDK. Link is here, https://www.oracle.com/technetwork/java/javase/downloads/jdk11-downloads-5066655.html
- Download JavaFX 11 SDK. https://gluonhq.com/products/javafx/. Unzip the contents to anywhere you like. I placed my in C:\Program Files\Java.

#### Running the Demo

1. Open Intellj and click import project. Import the folder name SeniorProjectDemo. Click next leave all options by default. It might ask you to overwrite .idea folder. You can click reuse. 
2. Once its imported, click on file, click on project structure. In project click Project SDK: java 11. If its not listed click on new. Find your java 11 folder. It should look something like this. C:\Program Files\Java\jdk-11.0.2. After you find it click apply. 
3. Because JavaFX is no longer included with java 11. You have to add it modules. So inside project structure. Inside Global Libraries click on the plus sign and click on java. Find your javafx folder lib folder. For me it looks like C:\Program Files\Java\javafx-sdk-11.0.2\lib.
4. Inside the lib folder. Highlight and click ok for all the .jar files. 
5. Intellj will ask you if you want to add these jar files in SeniorProjectDemo modules. Click yes.
6. The name of the library will be call javafx-swt. I renamed my to javafx11. 
7. Now go to Modules in project structure. Click on Dependencies. You should see javafx-swt or java11 if you renamed it.  Note: Every time you import a javafx project you should go to project structure and inside Global Libraries right click on java11 library and click add to modules for the specific project. 
8. Make sure your SDK is java 11.  Hit apply.

- Errors I encountered doing these steps on my desktop. 

- There is no run button.

  ```Java
  public class Main extends Application{
      //There should be a green play button next to public.
  }
  ```

  - I used this fix. Right click on **src** directory and select option **Mark Directory As** > **Sources Root**.

- Another error I encountered was. Cannot start compilation: the output path is not specified for module "SeniorProjectDemo". Specify the output path in the Project Structure dialog. Here is the fix I used. You have to define the `project compiler output` in `File` -> `Project Structure...` -> `Project` -> `Project compiler output`: The folder should look like this. C:\Users\Jonathan\IdeaProjects\SeniorProjectDemo\out



​    

​    


​      







