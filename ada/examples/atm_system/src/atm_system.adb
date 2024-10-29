with Ada.Text_IO;
with Ada.Numerics.Float_IO;
with Ada.Synchronous_Task_Control;

procedure Atm_System is
   protected Bank_Account is
      procedure Deposit (Amount : Float);
      procedure Withdraw (Amount : Float);
      function Get_Balance return Float;
   private
      Balance : Float := 0.0;  -- Initial balance
   end Bank_Account;

   protected body Bank_Account is
      procedure Deposit (Amount : Float) is
      begin
         Balance := Balance + Amount;
      end Deposit;

      procedure Withdraw (Amount : Float) is
      begin
         if Amount <= Balance then
            Balance := Balance - Amount;
         else
            Ada.Text_IO.Put_Line ("Insufficient funds for withdrawal.");
         end if;
      end Withdraw;

      function Get_Balance return Float is
      begin
         return Balance;
      end Get_Balance;
   end Bank_Account;

   task type Deposit_Transaction (Amount : Float);
   task body Deposit_Transaction is
   begin
      Bank_Account.Deposit (Amount);
      Ada.Text_IO.Put_Line ("Deposited: "
        & Float'Image (Amount)
        & ", New Balance: "
        & Float'Image (Bank_Account.Get_Balance));
   end Deposit_Transaction;

   task type Withdraw_Transaction (Amount : Float);
   task body Withdraw_Transaction is
   begin
      Bank_Account.Withdraw (Amount);
      Ada.Text_IO.Put_Line ("Withdrew: "
        & Float'Image (Amount)
        & ", New Balance: "
        & Float'Image (Bank_Account.Get_Balance));
   end Withdraw_Transaction;

   task Main is
      D1 : Deposit_Transaction(100.0);
      D2 : Deposit_Transaction(50.0);
      W1 : Withdraw_Transaction(30.0);
      W2 : Withdraw_Transaction(150.0);
   end Main;

begin
   null;
end Atm_System;
