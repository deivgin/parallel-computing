with Ada.Text_IO;
with Ada.Interrupts;

procedure Dining_Philosophers is

   Num_Philosophers : constant Integer := 5;

   protected Fork is
      entry Pick_Up;   -- Entry for picking up the fork
      entry Put_Down;  -- Entry for putting down the fork
   private
      Available : Boolean := True;  -- Indicates if the fork is available
   end Fork;

   protected body Fork is
      entry Pick_Up when Available is
      begin
         Available := False;  -- Mark the fork as unavailable
      end Pick_Up;

      entry Put_Down when not Available is
      begin
         Available := True;  -- Mark the fork as available
      end Put_Down;
   end Fork;

   -- Array of forks
   Forks : array (0 .. Num_Philosophers - 1) of Fork;

   -- Philosopher task
   task type Philosopher(Id: Integer) is
   end Philosopher;

   -- Task body for each philosopher
   task body Philosopher is
   begin
      loop
         -- Think
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " is thinking.");
         delay 1.0;  -- Simulate thinking time

         -- Pick up left fork
         Forks(Id).Pick_Up;
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " picked up left fork.");

         -- Pick up right fork
         Forks((Id + 1) mod Num_Philosophers).Pick_Up;
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " picked up right fork.");

         -- Eat
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " is eating.");
         delay 1.0;  -- Simulate eating time

         -- Put down right fork
         Forks((Id + 1) mod Num_Philosophers).Put_Down;
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " put down right fork.");

         -- Put down left fork
         Forks(Id).Put_Down;
         Ada.Text_IO.Put_Line("Philosopher " & Integer'Image(Id) & " put down left fork.");
      end loop;
   end Philosopher;

   -- Create philosopher tasks
   Philosopher_Instances : array (0 .. Num_Philosophers - 1) of Philosopher;

begin
   null;  -- Main program body can be empty
end Dining_Philosophers;
