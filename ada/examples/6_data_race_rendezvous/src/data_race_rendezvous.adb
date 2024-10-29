with Ada.Text_IO; use Ada.Text_IO;

procedure Data_Race is
   Shared_Counter : Integer := 0;

   task T1 is
      entry Increment_Counter;
   end T1;

   task body T1 is
   begin
      loop
         accept Increment_Counter do
            Shared_Counter := Shared_Counter + 1;
            Put_Line ("Task T1 incremented counter to: " & Integer'Image(Shared_Counter));
         end Increment_Counter;
      end loop;
   end T1;

begin
   Put_Line ("In main, counter starts at: " & Integer'Image(Shared_Counter));

   -- Main loop to increment the counter 10 times
   for I in 1 .. 10 loop
      T1.Increment_Counter;  -- Synchronized call to increment counter
   end loop;

   Put_Line ("In main, final counter value: " & Integer'Image(Shared_Counter));
end Data_Race;
