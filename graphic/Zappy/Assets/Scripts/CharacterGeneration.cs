﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterGeneration : MonoBehaviour
{
    public GameObject character;
    public RuntimeAnimatorController runtimeAnimator;

    private Animator animator;

    private Vector3 pos;
    private Quaternion rotation;

    // Start is called before the first frame update
    void Start()
    {
        animator = character.gameObject.GetComponent<Animator>();
        animator.runtimeAnimatorController = runtimeAnimator;
        pos.x = 0f;
        pos.y = 0f;
        pos.z = 0f;
        rotation.x = 0;
        rotation.y = 10f;
        rotation.z = 0;
        character.transform.localScale = new Vector3(1.5f, 1.5f, 1.5f);
        for (int i = 0; i < 6 ; i++)
        {
            pos.x = Random.Range(-5f, 5f);
            pos.z = Random.Range(-5f, 5f);
            Instantiate(character , pos, rotation);
        }

    }

    // Update is called once per frame
    void Update()
    {
    }
}