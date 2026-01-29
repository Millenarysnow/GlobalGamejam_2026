// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Skills/SkillExecutionContext.h"
#include "GameFramework/Actor.h"

void FSkillExecutionContext::PushStack(FSkillStackFrame Frame)
{
	Stack.Add(Frame);
}

FSkillStackFrame FSkillExecutionContext::PopStack()
{
	if (Stack.IsEmpty()) return FSkillStackFrame();
	return Stack.Pop();
}

FSkillStackFrame& FSkillExecutionContext::TopStackRef()
{
	checkf(!Stack.IsEmpty(), TEXT("Millenarysnow : SkillExecutionContext Stack is Empty when call TopStackRef"));
	
	return Stack.Top();
}

bool FSkillExecutionContext::IsStackEmpty() const
{
	return Stack.IsEmpty();
}

AActor* FSkillExecutionContext::GetTarget() const
{
	return Target.Get();
}

void FSkillExecutionContext::SetTarget(AActor* _Target)
{
	Target = _Target;
}
