with Ada.Text_IO; use Ada.Text_IO;

procedure Protected_Data_Race is
   protected Shared_Resource is
      procedure Increment;
      function Get_Value return Integer;
   private
      Counter : Integer := 0;
   end Shared_Resource;

   protected body Shared_Resource is
      procedure Increment is
      begin
         Counter := Counter + 1;
      end Increment;

      function Get_Value return Integer is
      begin
         return Counter;
      end Get_Value;
   end Shared_Resource;

   task T1;

   task body T1 is
   begin
      for I in 1 .. 10 loop
         Shared_Resource.Increment;
         Put_Line ("Task T1 incremented counter to: "
          & Integer'Image (Shared_Resource.Get_Value));
      end loop;
   end T1;

begin
   Put_Line ("In main, counter starts at: "
    & Integer'Image (Shared_Resource.Get_Value));
   Put_Line ("In main, final counter value: "
    & Integer'Image (Shared_Resource.Get_Value));
end Protected_Data_Race;
