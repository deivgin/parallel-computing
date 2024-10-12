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
