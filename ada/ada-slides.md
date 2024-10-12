---
marp: true
---

# Parallel computing with Ada programming language

Deividas Gineitis
Dominykas Pošiūnas

---

## The history of Ada (1/2)

- The Department of Defense (Dod) study in the early and middle 1970s indicated that enormous saving in software costs (about 24 billion. $ between 1983 and 1999) might be achieved if the Dod used one common programming language for all its applications instead of 450 programming languages and incompatible dialects used by its programmers.
- An international competition was held to design a language based on Dod’s requirements.

---

## The history of Ada (2/2)

- The winning proposal was programming language, originally developed in the early 1980s by a team led by Dr. Jean Ichbiah in France. With some minor modifications, this language referred to as **Ada** was adopted as an American National Standards Institute (ANSI) standard in 1983 (Ada 83).
- Major Ada versions include Ada 83, Ada 95, Ada 05 and Ada 12 (the most recent).
- The name “Ada” is not an acronym. It was chosen in honor of Augusta Ada Lovelace (1815-1852), a mathematician who is sometimes regarded as the world’s first programmer.

---

## Ada major features

- Ada is a structured, statically (and strong) typed, imperative, and multi-paradigm high-level programming language.
- Designed for embedded and real-times systems, focused on making bugs almost non-existent.
- Big five structural elements:
  - Packages - groups, units of compilations
  - Subprograms - procedures, functions - reusable sequences of instructions
  - Generics - arbitrary type packages that meet some requirement
  - Tasks - operations done in parallel
  - Protected objects - coordinate shared data

---

## Common uses

- Avionics (e.g. Airbus A380 flight control and navigation systems).
- Railways (e.g. Paris Metro).
- Space Technology (e.g. European Space Agency satellite control software).
- Military (e.g. Weapon systems, radar, and control systems).
- Banking (e.g. Transaction Processing).

---

## Hello World

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Greet is

begin
   Put_Line ("Hello, World!");

end Greet;
```

---

## Concurrency

Ada has a built in concurrency feature that can be achieved with `Tasking`

Tasking consists of two main parts:

1. Tasks
2. Protected objects

---

## Tasks

- Tasks are applications that are run concurrently to the main application. In other languages they might be called `Threads` and tasking itself can be called `multithreading`.
- Tasks are synchronized with the main application, but can process information separately from the main Task.
- The main application itself is a task, often called "master task" and tasks defined in it can be called subtasks.

---

## Task example

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Tasking is
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
end Tasking;

```

---

## Synchronization

- While master task and its subtasks are executed separately, the master task does not terminate until all of his subtasks have finished executing.
- This provides simple synchronization between master task and subtasks.
- The master task will wait for tasks in packages to execute before terminating.

---

### Synchronization example (1/2)

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
end Show_Simple_Sync;
```

---

### Synchronization example (2/2)

```ada
with Ada.Text_IO; use Ada.Text_IO;

package body Sub_Task is
   task body T is
   begin
      for I in 1 .. 10 loop
         Put_Line ("hello");
      end loop;
   end T;
end Sub_Task;
```

```ada
with Sub_Task;

procedure Main is
begin
   null;
end Main;
```

---

### Data race condition in Ada

Data race condition - two or more threads or processes access the same shared memory or data simultaneously.

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Data_Race is
   Shared_Counter : Integer := 0;

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

## Rendezvous synchronization (1/2)

- With ada we not only get automatic synchronization, but we are also able to control and define it at will.
- This can be done with custom synchronization points using the keyword `entry`.
- In the task body, you specify where the task will accept entry calls by using the keyword `accept`.
- For each `entry` point there is corresponding `accept` statement.

---

### Rendezvous synchronization (2/2)

Tasks run independently until either:

- An accept statement.
  Waits for someone to call this entry, then proceeds to the rendezvous section.
  After this, both tasks execute their ways.
- An entry call.
  Waits for corresponding task reaching its accept statement, then proceeds to the rendezvous section. After this, both tasks execute their ways.

This is synchronous communication.

---

### 'entry-accept' synchronization example (1/2)

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Rendezvous is
   task T is
      entry Start;
   end T;

   task body T is
   begin
      accept Start;
      Put_Line ("In T");
   end T;

begin
   Put_Line ("In Main");
   T.Start;
end Show_Rendezvous;
```

---

### 'entry-accept' synchronization example (2/2)

```ada
with Ada.Text_IO; use Ada.Text_IO;
with Ada.Float_Text_IO; use Ada.Float_Text_IO;

procedure Temperature_Converter is
   Celsius_Temperature : Float;
   Fahrenheit_Temperature : Float;

   type Temperature_Array is array (1 .. 5) of Float;
   Fixed_Temperatures : Temperature_Array := (0.0, 20.0, 37.5, 15.0, 30.0);

   task Producer is
      entry Produce (Temp : out Float);
   end Producer;

   task Consumer is
      entry Consume (Temp : Float);
   end Consumer;

   task body Producer is
   begin
      for I in Fixed_Temperatures'Range loop
         accept Produce (Temp : out Float) do
            Celsius_Temperature := Fixed_Temperatures (I);
            Put ("Produced Temperature: ");
            Put (Celsius_Temperature, Fore => 1, Aft => 1, Exp => 0);
            Put_Line (" °C");
            Temp := Celsius_Temperature;
         end Produce;
      end loop;
   end Producer;

   task body Consumer is
   begin
      for I in Fixed_Temperatures'Range loop
         accept Consume (Temp : Float) do
            Fahrenheit_Temperature := (Temp * 9.0 / 5.0) + 32.0;
            Put ("Consumed Temperature: ");
            Put (Temp, Fore => 1, Aft => 1, Exp => 0);
            Put (" °C -> ");
            Put (Fahrenheit_Temperature, Fore => 1, Aft => 1, Exp => 0);
            Put_Line (" °F");
         end Consume;
      end loop;
   end Consumer;

begin
   for I in Fixed_Temperatures'Range loop
      declare
         Temp : Float;
      begin
         Producer.Produce (Temp);
         Consumer.Consume (Temp);
      end;
   end loop;

   Put_Line ("Temperature conversion completed.");
end Temperature_Converter;

```

---

### Terminate and delay

TODO
paimti iš 4 reference

---

## Protected objects

- In Ada, protected objects are a concurrency control mechanism designed to safely encapsulate and manage shared data.
- Protected objects encapsulate data in their private part. Data is shared among tasks but can only be accessed via protected operations.
- Ada ensures that when a task is executing a protected operation, no other task can interfere, making it thread-safe by default.
- Protected procedures and functions are similar to getters and setters in object-oriented programming.

---

### Protected object example (1/2)

```ada
protected Obj is
    procedure Set(Value : Integer);
    function Get return Integer;
private
    Local : Integer;
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

### Protected object example (2/2)

```ada
with Ada.Text_IO; use Ada.Text_IO;

procedure Protected_Object is

   protected Obj is
      procedure Set (V : Integer);
      entry Get (V : out Integer);
   private
      Local  : Integer;
      Is_Set : Boolean := False;
   end Obj;

   protected body Obj is
      procedure Set (V : Integer) is
      begin
         Local := V;
         Is_Set := True;
      end Set;

      entry Get (V : out Integer)
        when Is_Set is
      begin
         V := Local;
         Is_Set := False;
      end Get;
   end Obj;

   N : Integer := 0;

   task T;

   task body T is
   begin
      Put_Line
        ("Task T will delay for 4 seconds...");
      delay 4.0;

      Put_Line
        ("Task T will set Obj...");
      Obj.Set (5);

      Put_Line
        ("Task T has just set Obj...");
   end T;
begin
   Put_Line
     ("Main application will get Obj...");
   Obj.Get (N);

   Put_Line
     ("Main application has retrieved Obj...");
   Put_Line
     ("Number is: " & Integer'Image (N));

end Protected_Object;

```

---

## References

[1] <https://www.adacore.com/about-ada>
[2] <https://learn.adacore.com/courses/intro-to-ada/chapters/tasking.html>
[3] <https://youtu.be/YPD9U4Wuh5A?si=YxNWNLj57tAQoIne>
[4] <https://www.youtube.com/watch?v=ZcdCDEhkbjU>
[5] <https://www.youtube.com/watch?v=RjbrUbp1Xo4>
