with Ada.Text_IO; use Ada.Text_IO;

procedure Task_Type is
   task type TT;

   task body TT is
   begin
      Put_Line ("In task type TT");
   end TT;

   A_Task : TT;
begin
   Put_Line ("In main");
end Task_Type;
