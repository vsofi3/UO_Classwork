(*  Concrete syntax
e :: x | n | true | false | succ | pred | iszero | if e then e else e
       | fn x => e | e e | (e) | let x = e in e

Abstract Syntax
datatype term = AST_ID of string | AST_NUM of int | AST_BOOL of bool
  | AST_SUCC | AST_PRED | AST_ISZERO | AST_IF of (term * term * term)
  | AST_FUN of (string * term) | AST_APP of (term * term)
  | AST_LET of (string * term * term)
  | AST_ERROR of string
*)

use "parser.sml";

(*datatype result = RES_ERROR of string | RES_NUM of int| RES_BOOL  of bool
                | RES_SUCC | RES_PRED | RES_ISZERO | RES_FUN of (string * term)
                | RES_CLOSURE of (string * term * env) and env = Env of (string -> term * result);
*)
datatype result = RES_ERROR of string | RES_NUM of int| RES_BOOL  of bool
                | RES_SUCC | RES_PRED | RES_ISZERO | RES_FUN of (string * term)
                | RES_CLOSURE of (string * term * env2) and env2 = Env2 of (string -> term * env2);

exception UnboundID
datatype env = Env of (string -> term)
(*datatype env2 = Env of (string -> term * env)*)

fun emptyenvFun  (x : string) : term = raise UnboundID;
val emptyenv = Env emptyenvFun
fun update (Env e) (x : string) (ty : term) = fn y => if x = y then ty else e y

fun emptyenvFunStat (x : string) : term * env2 = raise UnboundID;
val emptyenvStat = Env2 emptyenvFunStat
fun updateStat (Env2 e) (x : string) (ty : term * env2) = fn y => if x = y then ty else e y 
exception Not_implemented_yet
exception Error of string


fun interp (Env env, AST_ID i)      = interp(Env env, env i) 
  | interp (env, AST_NUM n)         = RES_NUM n
  | interp (env, AST_BOOL b)        = RES_BOOL b
  | interp (env, AST_FUN (i,e))     = RES_FUN (i,e)
  | interp (env, AST_APP (AST_APP (AST_ADD, e1), e2)) = 
                       (case interp (env, e1) of
                          RES_NUM n1 => (case interp (env, e2) of
                                          RES_NUM n2 => RES_NUM (n1+n2)
                                        |  _         => raise (Error  "not a number"))
                        |  _         => raise (Error "not a number")
                       )
  | interp (env, AST_APP (AST_ADD, e1)) = raise (Error "not enough arguments for +")
  | interp (env, AST_APP (e1,e2))   = (case interp (env, e1) of
         RES_FUN (x, body) => let val newEnv = (update env x e2)
                              in 
                                    interp(Env newEnv, body)
                              end
       | RES_SUCC =>  (case (interp(env, e2)) of 
                           RES_NUM n => RES_NUM (n+1)
                        | _ => RES_ERROR "Type Error: Can't take succ of non number")
       | RES_PRED =>  (case (interp(env, e2)) of 
                           RES_NUM 0 => RES_NUM 0
                        |  RES_NUM n => RES_NUM (n-1)
                        | _ => RES_ERROR "Type Error: Can't take pred of a non number") 
       | RES_ISZERO => (case (interp(env, e2)) of 
                           RES_NUM 0 => RES_BOOL true
                        |  RES_NUM n => RES_BOOL false
                        | _ => RES_ERROR "Type Error: Can't check if non number is 0")
       | _ => raise Error "apply non-function")
  | interp (env, AST_SUCC)          = RES_SUCC
  | interp (env, AST_PRED)          = RES_PRED
  | interp (env, AST_ISZERO)        = RES_ISZERO
  | interp (env, AST_IF (e1,e2,e3)) =  (case interp (env,e1) of
                                     RES_BOOL true  => interp (env,e2)
                                   | RES_BOOL false => interp (env,e3)
                                   | _              => raise Error  "case on non-bool")

  | interp (env, AST_LET (x,e1,e2)) = let val newEnv = (update env x e1)
                                      in
                                            interp(Env newEnv, e2)
                                      end 
  | interp (env, AST_ERROR s)       = raise (Error s) 

fun interpStat (Env2 env, AST_ID i)     = let val (trm, cal) = env i
                                          in 
                                                interpStat(cal, trm) 
                                          end 
  | interpStat (env, AST_NUM n)         = RES_NUM n
  | interpStat (env, AST_BOOL b)        = RES_BOOL b
  | interpStat (env, AST_FUN (i,e))     = RES_CLOSURE(i, e, env)
  | interpStat (env, AST_APP (AST_APP (AST_ADD, e1), e2)) = 
                       (case interpStat (env, e1) of
                          RES_NUM n1 => (case interpStat (env, e2) of
                                          RES_NUM n2 => RES_NUM (n1+n2)
                                        |  _         => raise (Error  "not a number"))
                        |  _         => raise (Error "not a number")
                       )
  | interpStat (env, AST_APP (AST_ADD, e1)) = raise (Error "not enough arguments for +")
  | interpStat (env, AST_APP (e1,e2))   = (case interpStat (env, e1) of
         RES_CLOSURE (i, e, env2) => let 
                                          val newEnv = (updateStat env2 i (e2, env))
                                     in 
                                          interpStat(Env2 newEnv, e)
                                     end
       | RES_SUCC =>  (case (interpStat(env, e2)) of 
                           RES_NUM n => RES_NUM (n+1)
                        | _ => RES_ERROR "Type Error: Can't take succ of non number")
       | RES_PRED =>  (case (interpStat(env, e2)) of 
                           RES_NUM 0 => RES_NUM 0
                        |  RES_NUM n => RES_NUM (n-1)
                        | _ => RES_ERROR "Type Error: Can't take pred of a non number") 
       | RES_ISZERO => (case (interpStat(env, e2)) of 
                           RES_NUM 0 => RES_BOOL true
                        |  RES_NUM n => RES_BOOL false
                        | _ => RES_ERROR "Type Error: Can't check if non number is 0")
       | _ => raise Error "apply non-function")
  | interpStat (env, AST_SUCC)          = RES_SUCC
  | interpStat (env, AST_PRED)          = RES_PRED
  | interpStat (env, AST_ISZERO)        = RES_ISZERO
  | interpStat (env, AST_IF (e1,e2,e3)) =  (case interpStat (env,e1) of
                                     RES_BOOL true  => interpStat (env,e2)
                                   | RES_BOOL false => interpStat (env,e3)
                                   | _              => raise Error  "case on non-bool")

  | interpStat (env, AST_LET (x,e1,e2)) = let val newEnv = (updateStat env x (e1, env))
                                      in
                                            interpStat(Env2 newEnv, e2)
                                      end 
  | interpStat (env, AST_ERROR s)       = raise (Error s) 



fun eval s = interp (emptyenv, parsestr s)
              handle (Error z) => RES_ERROR z;

fun evalStat s = interpStat (emptyenvStat, parsestr s)
                 handle (Error z) => RES_ERROR z;

(********************************* 4 *********************************
    If the input had been typechecked before being passed into the interpreter, the interpreter 
    itself would not be required to handle input for non-numbers, non-boolean, or non-function values 
    like it currently does.   
*)
