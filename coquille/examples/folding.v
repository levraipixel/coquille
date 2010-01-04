Section testOfSectionFolding.

(*
Test of commentary folding
*)


Lemma testOfProofFolding : 2=2.
Proof.
Show.
Lemma testOfNestedProofFolding : 3=3.
Proof.
reflexivity.
Defined.
reflexivity.
Defined.
Qed.

End testOfSectionFolding.
