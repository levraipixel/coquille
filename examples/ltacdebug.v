Set Ltac Debug.

Require Import Omega.

Lemma truc : 4 <= 6.
omega.
do 3 try( reflexivity ).
admit.
