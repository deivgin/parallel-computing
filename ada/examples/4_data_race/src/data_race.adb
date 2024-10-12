with Ada.Text_IO; use Ada.Text_IO;

procedure Data_Race is
   Shared_Counter : Integer := 0;

   task T1;

   task body T1 is
   begin
      for I in 1 .. 10 loop
         Shared_Counter := Shared_Counter + 1;
         Put_Line ("Task T1 incremented counter to: "
          & Integer'Image (Shared_Counter));
      end loop;
   end T1;

begin
   Put_Line ("In main, counter starts at: "
    & Integer'Image (Shared_Counter));
   delay 1.0;
   Put_Line ("In main, final counter value: "
    & Integer'Image (Shared_Counter));
end Data_Race;
