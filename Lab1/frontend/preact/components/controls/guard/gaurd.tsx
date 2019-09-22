import { h } from 'preact';
import { auth, googleAuthProvider } from '../../firebase/firebase';
import { Button } from 'preact-fluid';

import style from './gaurd.scss';

const theme = {
    primaryColor: '#ffffff'
};

const Gaurd = () => (
    <div class={style.Stage}>
        <div class={style.Over}>
            <h3>Sign in to access remote sensor controls</h3>
            <Button class={style.Button} theme={theme} size={'large'} primary onClick={() => auth.signInWithRedirect(googleAuthProvider)}>
                Sign In
            </Button>
        </div>
        <div class={style.Middle} />
        <div class={style.Under}>
            <h2>Controls Hidden</h2>
        </div>
    </div>
);

export default Gaurd;
