---
marp: true
---

# Concurrency in Ada

Deividas Gineitis
Dominykas Pošiūnas

---

## Content

- Ada history
- Main Ada features
- Basic Ada syntax
- Multithreading concepts in Ada
- Data race condition
- Automatic synchronization
- Custom synchronization 'rendezvous'
- Producer-consumer problem
- ...

---

## Ada history

- Ada was originally developed in the early 1980s by a team led by Dr. Jean Ichbiah in France;
- Major Ada versions include: Ada 83, Ada 95, Ada 2005 and Ada 2012 (the most recent);
- The name “Ada” is not an acronym; it was chosen in honor of Augusta Ada Lovelace (1815-1852), a mathematician who is sometimes regarded as the world’s first programmer;

---

### Main Ada features

structured
statically typed
imperative
high-level
object-oriented

---

### Multithreading in Ada

- A thread in Ada is called "Task".
- Multithreading in Ada might be called "Tasking".
- Tasks may synchronize with the main application but may also process information completely independently from the main application.
- Tasks are declared using the keyword *task*.
- The task implementation is specified in a *task body* block.

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

- Custom synchronization points can be defined with keyword *entry*.
- Entry points are defined in the task initialization. For example:

```ada
task T is
   entry Start;
end T;
```

- In the task body, you specify where the task will accept these entries by using the keyword *accept*. For example:

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
- When the main program calls *T.Start*, it synchronizes with the accept statement in task T.
- *accept Start* is where T waits for entry call from the main procedure.
- The result - "In Main" will always be printed before "In T"

---

## References

[1] <https://www.adacore.com/about-ada>
[2]
[3]
[4]
[5]
[6]
