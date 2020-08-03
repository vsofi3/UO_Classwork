(*  Concrete syntax 
e :: x | n | true | false | succ | pred | iszero | if e then e else e 
       | fn x => e | e e | (e) | let x = e in e 

Ambiguous
 - How do you parse e f g  (e f) g (x) or  e (f g )
 - How do you parse fn f = f 0   (fn f = f) 0 or fn f = (f 0) 

Abstract syntax tree :
datatype term = AST_ID of string | AST_NUM of int | AST_BOOL of bool
  | AST_SUCC | AST_PRED | AST_ISZERO | AST_ADD |  AST_IF of (term * term * term)
  | AST_FUN of (string * term) | AST_APP of (term * term) 
  | AST_LET of (string * term * term) 
  | AST_ERROR of string
*)

use "parser.sml";

datatype result = RES_ERROR of string | RES_NUM of int| RES_BOOL  of bool
                | RES_SUCC | RES_PRED | RES_ISZERO | RES_FUN of (string * term)
                | RES_CLOSURE of (string * term * env) and env = Env of (string -> result);

(*  An environment is a function string -> result  *)

exception UnboundID
exception Error of string

(*datatype env = Env of (string -> result) - Commenting this out because it is now defined in result*)

fun emptyenvFun  (x : string) : result = raise UnboundID;
val emptyenv = Env emptyenvFun

(*  update : env -> string -> result -> string -> result  *)
fun update (Env e) (x : string) (ty : result) = fn y => if x = y then ty else e y

fun interp (env, AST_ID i)          = let val Env e = env in e i end 
  | interp (env, AST_NUM n)         = RES_NUM n
  | interp(env, AST_BOOL b)        = RES_BOOL b
  | interp (env, AST_FUN (i,e))     = RES_FUN (i,e)
  | interp (env, AST_APP (AST_APP (AST_ADD, e1), e2)) = 
                       (case interp (env, e1) of
                          RES_NUM n1 => (case interp (env, e2) of
                                          RES_NUM n2 => RES_NUM (n1+n2)
                                        |  _         => raise (Error  "not a number"))
                        |  _         => raise (Error "not a number")
                       )
  | interp (env, AST_APP (AST_ADD, e1)) = raise (Error "not enough arguments for +")
  | interp (env, AST_APP (e1,e2))   =  (case interp (env, e1) of
           RES_SUCC => (case (interp(env, e2)) of 
                          RES_NUM n => RES_NUM (n + 1)
                        | _ => RES_ERROR "Type Error: Can't take succ of a non number")
         | RES_PRED => (case (interp(env, e2)) of
                           RES_NUM 0 => RES_NUM 0 
                        |  RES_NUM n => RES_NUM(n - 1)
                        | _ => RES_ERROR "Type Error: Can't take pred of a non number")
         | RES_ISZERO => (case (interp(env, e2)) of 
                            RES_NUM 0 => RES_BOOL true
                          | RES_NUM n => RES_BOOL false
                          | _ => RES_ERROR "Type Error: Can't check if a non number is 0")    
         | RES_FUN (v, body) => let val v2 = interp (env,e2)
                               in let val new_env = update env v v2
                                   in interp (Env new_env, body)
                                   end
                              end  
         | _ => raise (Error "Not Implemented yet!") )

  | interp (env, AST_SUCC)          = RES_SUCC 
  | interp (env, AST_PRED)          = RES_PRED 
  | interp (env, AST_ISZERO)        = RES_ISZERO 
  | interp (env, AST_IF (e1,e2,e3)) = (case interp (env,e1) of
                                              RES_BOOL true => interp (env,e2)
                                            | RES_BOOL false => interp (env,e3)
                                            | _              => raise 
                                                          (Error "if condition non-bool!") )
  | interp (env, AST_LET (x,e1,e2)) = let val v1 = interp(env, e1) 
                                        in case v1 of
                                              RES_ERROR s => v1
                                            | _ => let val newEnv = update env x v1 
                                                   in 
                                                     interp( Env newEnv, e2)
                                                   end
                                       end     
  | interp (env, AST_ERROR s)       = raise (Error s)

fun interp_dynamic s = interp (emptyenv, parsestr s)
                     handle (Error z) => RES_ERROR z ;


fun interp_stat (env, AST_ID i)          = let val Env e = env in e i end 
  | interp_stat (env, AST_NUM n)         = RES_NUM n
  | interp_stat (env, AST_BOOL b)        = RES_BOOL b
  | interp_stat (env, AST_FUN (i,e))     = RES_CLOSURE(i, e, env)
  | interp_stat (env, AST_APP (AST_APP (AST_ADD, e1), e2)) = 
                       (case interp_stat (env, e1) of
                          RES_NUM n1 => (case interp_stat (env, e2) of
                                          RES_NUM n2 => RES_NUM (n1+n2)
                                        |  _         => raise (Error  "not a number"))
                        |  _         => raise (Error "not a number")
                       )
  | interp_stat (env, AST_APP (AST_ADD, e1)) = raise (Error "not enough arguments for +")
  | interp_stat (env, AST_APP (e1,e2))   =  (case interp_stat (env, e1) of
           RES_SUCC => (case (interp_stat(env, e2)) of 
                          RES_NUM n => RES_NUM (n + 1)
                        | _ => RES_ERROR "Type Error: Can't take succ of a non number")
         | RES_PRED => (case (interp_stat(env, e2)) of
                           RES_NUM 0 => RES_NUM 0 
                        |  RES_NUM n => RES_NUM(n - 1)
                        | _ => RES_ERROR "Type Error: Can't take pred of a non number")
         | RES_ISZERO => (case (interp_stat(env, e2)) of 
                            RES_NUM 0 => RES_BOOL true
                          | RES_NUM n => RES_BOOL false
                          | _ => RES_ERROR "Type Error: Can't check if a non number is 0")    
         | RES_CLOSURE (i, e, env1) => 
                let
                    val v1 = interp_stat(env, e2)
                    in 
                        let 
                            val newEnv = update env1 i v1
                        in 
                            interp_stat(Env newEnv, e)
                        end
                end

         | _ => raise (Error "Not Implemented yet!") ) 

  | interp_stat (env, AST_SUCC)          = RES_SUCC 
  | interp_stat (env, AST_PRED)          = RES_PRED 
  | interp_stat (env, AST_ISZERO)        = RES_ISZERO 
  | interp_stat (env, AST_IF (e1,e2,e3)) = (case interp_stat (env,e1) of
                                              RES_BOOL true => interp_stat (env,e2)
                                            | RES_BOOL false => interp_stat (env,e3)
                                            | _              => raise 
                                                          (Error "if condition non-bool!") )
  | interp_stat (env, AST_LET (x,e1,e2)) = let val v1 = interp_stat(env, e1) 
                                        in case v1 of
                                              RES_ERROR s => v1
                                            | _ => let val newEnv = update env x v1 
                                                   in 
                                                     interp_stat( Env newEnv, e2)
                                                   end
                                       end     
  | interp_stat (env, AST_ERROR s)       = raise (Error s)
