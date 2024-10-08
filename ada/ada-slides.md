---
marp: true
---

# Ada's task and synchonization capabilites

Deividas Gineitis
Dominykas Pošiūnas

---

## Content

- Ada history.
- Ada features, usage and syntax.
- Ada's task and synchonization capabilites.

---

### The history of Ada 1/2

- The Department of Defense (Dod) study in the early and middle 1970s indicated that enormous saving in software costs (about 24 billion. $ between 1983 and 1999) might be achieved if the Dod used one common programming language for all its applications instead of 450 programming languages and incompatible dialects used by its programmers.
- An international competition was held to design a language based on Dod’s requirements.

---

### The history of Ada 2/2

- The winning proposal was programming language, originally developed in the early 1980s by a team led by Dr. Jean Ichbiah in France. With some minor modifications, this language referred to as **Ada** was adopted as an American National Standards Institute (ANSI) standard in 1983 (Ada 83).
- Major Ada versions include Ada 83, Ada 95, Ada 05 and Ada 12 (the most recent).
- The name “Ada” is not an acronym. It was chosen in honor of Augusta Ada Lovelace (1815-1852), a mathematician who is sometimes regarded as the world’s first programmer.

---

### Ada major features

- Ada is a structured, statically typed, imperative, and object-oriented high-level programming language, inspired by Pascal and other languages.
- Designed for embedded and real-times systems, focused on making bugs almost non-existent.
- Major features of Ada include:
Strong typing, runtime checking, parallel processing (tasks, synchronous message passing), exception handling, OOP, polymorphism.

---

### Common uses

- Avionics (e.g. Airbus A380 flight control and navigation systems).
- Railways (e.g. Paris Metro).
- Space Technology (e.g. European Space Agency satellite control software).
- Military (e.g. Weapon systems, radar, and control systems).
- Banking (e.g. Transaction Processing).

---

<<<<<<< HEAD
### Main Ada features

TODO needs improvement
structured
statically typed
imperative
high-level

---

### The big five structural elements

Ada programs are organized with main 5 elements:

- Packages - groups, units of compilations
- Subprograms - procedures, functions - reusable sequences of instructions
- Generics - arbitrary type packages that meet some requirement
- Tasks - operations done in parallel
- Protected objects - coordinate shared data

---

=======
>>>>>>> db8e6b4 (Update history and add major features)
### Hello World

```ada
with Ada.Text_IO; use Ada.Text_IO;
-- The `with` clause makes the Ada.Text_IO package available in this procedure.
-- The `use` clause allows direct access to Text_IO's subprograms, like Put_Line,
-- without needing to prefix them with Ada.Text_IO.

procedure Greet is
-- This defines the procedure named `Greet`. It doesn't take any parameters.

begin
   -- The body of the procedure starts here.

   -- Print "Hello, World!" to the screen
   Put_Line ("Hello, World!");
   -- The Put_Line subprogram prints the string "Hello, World!" to the standard output (console).

end Greet;
-- The procedure ends here.
```

---

### Multithreading in Ada

- A thread in Ada is called "Task".
- Multithreading in Ada might be called "Tasking".
- Tasks may synchronize with the main application but may also process information completely independently from the main application.
- Tasks are declared using the keyword _task_.
- The task implementation is specified in a _task body_ block.

- The main application is itself a task and can be referenced "master task".

---

### Simple 'multithreading' example in Ada (1)

- master and T tasks runs concurrently.

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Simple_Task is
task T;

task body T is
begin
    Put_Line ("In task T");
end T;
begin
Put_Line ("In main");
end Show_Simple_Task;
```

---

### Simple 'multithreading' example in Ada (2)

- master, T and T2 tasks runs concurrently.

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Simple_Tasks is
   task T;
   task T2;

   task body T is
   begin
      Put_Line ("In task T");
   end T;

   task body T2 is
   begin
      Put_Line ("In task T2");
   end T2;

begin
   Put_Line ("In main");
end Show_Simple_Tasks;
```

---

### Data race condition in Ada

Data race condition - two or more threads or processes access the same shared memory or data simultaneously.

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Data_Race is
   Shared_Counter : Integer := 0;  -- Shared variable

   task T1;

   task body T1 is
   begin
      for I in 1 .. 10 loop
         Shared_Counter := Shared_Counter + 1;
         Put_Line ("Task T1 incremented counter to: " & Integer'Image(Shared_Counter));
      end loop;
   end T1;

begin
   Put_Line ("In main, counter starts at: " & Integer'Image(Shared_Counter));
   delay 1.0;
   Put_Line ("In main, final counter value: " & Integer'Image(Shared_Counter));
end Data_Race;

```

---

### Automatic synchronization

- Master task always waits until its subtasks have finished before it allows itself to complete.
- This can be considered as an automatic synchronization between the master task and its subtasks.

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Simple_Sync is
   task T;
   task body T is
   begin
      for I in 1 .. 10 loop
         Put_Line ("hello");
      end loop;
   end T;
begin
   null;
   --  Will wait here until all tasks
   --  have terminated
end Show_Simple_Sync;
```

---

### Custom synchronization 'rendezvous'

- Custom synchronization points can be defined with keyword _entry_.
- Entry points are defined in the task initialization. For example:

```ada
task T is
   entry Start;
end T;
```

- In the task body, you specify where the task will accept these entries by using the keyword _accept_. For example:

```ada
task body T is
begin
   accept Start;
   Put_Line ("In T");
end T;
```

---

### 'rendezvous' synchronization example

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Rendezvous is

   task T is
      entry Start;
   end T;

   task body T is
   begin
      accept Start;
      --     ^ Waiting for somebody
      --       to call the entry

      Put_Line ("In T");
   end T;

begin
   Put_Line ("In Main");

   --  Calling T's entry:
   T.Start;
end Show_Rendezvous;
```

---

### 'rendezvous' synchronization example explanation

- The task T is declared with an entry point called "Start".
- T will pause and wait for the main program to call the an entry point.
- When the main program calls _T.Start_, it synchronizes with the accept statement in task T.
- _accept Start_ is where T waits for entry call from the main procedure.
- The result - "In Main" will always be printed before "In T"

---

### Reader-writer or Consumer-producer or any other problem using 'rendevzous' synchronization

TODO

---

### Protected objects

In Ada, protected objects are a concurrency control mechanism designed to safely encapsulate and manage shared data.

```ada
protected Obj is
    procedure Set(Value : Integer);  -- Setter-like operation
    function Get return Integer;     -- Getter-like operation
private
    Local : Integer;                 -- Protected data
end Obj;

protected body Obj is
    procedure Set(Value : Integer) is
    begin
        Local := Value;
    end Set;

    function Get return Integer is
    begin
        return Local;
    end Get;
end Obj;
```

---

### Logic of Protected objects

- Protected objects encapsulate data in their private part. Data is shared among tasks but can only be accessed via protected operations.
- Ada ensures that when a task is executing a protected operation, no other task can interfere, making it thread-safe by default.
- Protected procedures and functions are similar to getters and setters in object-oriented programming.

---

### Protected objects in Java?

In Java, the concept closest to protected objects in Ada can be implemented using synchronized methods or synchronized blocks, which are used to control access to shared data among multiple threads

```java
public class SharedResource {
    private int value;

    // Synchronized setter (like Ada's protected procedure Set)
    public synchronized void setValue(int newValue) {
        this.value = newValue;
    }

    // Synchronized getter (like Ada's protected function Get)
    public synchronized int getValue() {
        return this.value;
    }
}
```

---

## References

[1] <https://www.adacore.com/about-ada>
[2] <https://learn.adacore.com/courses/intro-to-ada/chapters/tasking.html>
[3] <https://youtu.be/YPD9U4Wuh5A?si=YxNWNLj57tAQoIne>
[4] <https://www.youtube.com/watch?v=ZcdCDEhkbjU>
[5] <https://www.youtube.com/watch?v=RjbrUbp1Xo4>
[6]
