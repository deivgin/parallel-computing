with Ada.Text_IO; use Ada.Text_IO;

procedure Show_Simple_Task is
   task T;

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
end Show_Simple_Task;
