Lemma essai :
forall n : nat,
exists p : nat,
~exists q : nat,
n <= p /\ ( p >= q \/ q <> n).
